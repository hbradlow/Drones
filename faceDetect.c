#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <asm/types.h>
#include <linux/videodev2.h>
#include "faceDetect.h"
#include "ComWithMPU2.h"

//#define RESIZE_WITH_ARM
#ifdef RESIZE_WITH_ARM
#include <arm_neon.h>
#endif

//#define DEBUG

#ifdef DEBUG
#define DBG printf
#else
#define DBG(...) do{}while(0)
#endif

#define CLEAR(x) memset(&(x), 0, sizeof(x))

#define TRUE 1
#define FALSE 0
#define PATHDEVFD "/dev/video0"

struct buffer {
	void *start;
	size_t length;
};

struct GstFDSink {
	int fd;
	int width, height;
	int n_buffers;
	struct buffer *buffers;
	int output;
	int factor;
} GstFDSink;

struct GstFDSink gstfdsink;
struct GstFDSink *fdsink = &gstfdsink;

static void dump_v4l2_format(struct v4l2_format *fmt, int output) {
	if (output & OUTPUT_PRINTF)
		printf("%s type=%d w=%d h=%d pix=%x field=%d\n\n", __func__, fmt->type, fmt->fmt.pix.width, fmt->fmt.pix.height, fmt->fmt.pix.pixelformat, fmt->fmt.pix.field);
}
	
static void dump_v4l2_cap(struct v4l2_capability *cap, int output) {
	if (output & OUTPUT_PRINTF) {
		printf("%s: driver=%s card=%s bus_info=%s\n", __func__, cap->driver, cap->card, cap->bus_info);
		printf("%s: ver=%x cap=%x\n\n", "\t", cap->version, cap->capabilities);
	}
}

static void dump_fdif_results(struct v4l2_od_result *res, int output, int factor) {
	if (output & OUTPUT_PRINTF) {
	        int idx;
//		printf("%s\n", __func__);
//        	printf("frame seq=%d, found %d objects\n", res->frm_seq, res->obj_cnt);
	        for(idx = 0; idx < res->obj_cnt; idx++) {
        	        struct v4l2_od_object *fr = &res->od[idx];
			if (fr->type == V4L2_OD_TYPE_FACE)
		                printf("Id.%d x=%3d y=%3d szx=%3d szy=%3d ang=%3d conf=%2d\n", fr->o.face.id, fr->o.face.f.centerx, fr->o.face.f.centery, fr->o.face.f.sizex, fr->o.face.f.sizey, fr->o.face.f.angle, fr->confidence);
        	}
	}

	if (output & OUTPUT_VIDEO) {
		int fd = open("/sys/bus/i2c/drivers/ov5640/3-003c/rectangle", O_WRONLY);
		if (fd >= 0) {
			int ret = 0;
			char szvalue[256];
			if (res->obj_cnt == 0)
				sprintf(szvalue, "0 [0 0 0 0 0 0]");
			else {
			        struct v4l2_od_object *fr = &res->od[0];
				sprintf(szvalue, "1 [%d %d %d %d 10 10]", (fr->o.face.f.centerx - fr->o.face.f.sizex / 2) * factor, (fr->o.face.f.centery - fr->o.face.f.sizey / 2) * factor, fr->o.face.f.sizex * factor, fr->o.face.f.sizey * factor);
			}
			ret = write(fd, szvalue, strlen(szvalue));
			close(fd);
		}
	}

	if ((output & OUTPUT_MEMORY) && (res->obj_cnt > 0)) {
		struct structCVtoMPU2 data;
		data.type = TYPE_PILOT_FACE;
		data.face = res->od[0].o.face.f;
		CVtoMPU2_Send(0, &data);
	}
}

static int xioctl(int fd, int request, void *arg) {
	int r;
	do r = ioctl(fd, request, arg);
	while(-1 == r && EINTR == errno);
	return r;
}

static int uninit_device(struct GstFDSink *fdsink) {
	unsigned int i;

	for(i = 0; i < fdsink->n_buffers; ++i)
		if (-1 == munmap(fdsink->buffers[i].start, fdsink->buffers[i].length)) {
			fprintf(stderr, "munmap\n");
			return FALSE;
		}

	free(fdsink->buffers);

	return TRUE;
}

static int close_device(struct GstFDSink *fdsink) {
	if (-1 == close(fdsink->fd))
		return FALSE;
	fdsink->fd = -1;
	return TRUE;
}

static int open_device(struct GstFDSink *fdsink) {
	struct stat st;

	if (-1 == stat(PATHDEVFD, &st)) {
		fprintf(stderr, "Cannot identify '%s': %d, %s\n", PATHDEVFD, errno, strerror(errno));
		return FALSE;
	}

	if (!S_ISCHR(st.st_mode)) {
		fprintf(stderr, "%s is no device\n", PATHDEVFD);
		return FALSE;
	}

	fdsink->fd = open(PATHDEVFD, O_RDWR | O_NONBLOCK, 0);

	if (-1 == fdsink->fd) {
		fprintf(stderr, "Cannot open '%s': %d, %s\n", PATHDEVFD, errno, strerror(errno));
		return FALSE;
	}
	return TRUE;
}

static int init_mmap(struct GstFDSink *fdsink) {
	struct v4l2_requestbuffers req;

	CLEAR(req);

	req.count = 1;
	req.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
	req.memory = V4L2_MEMORY_MMAP;

	if (-1 == xioctl(fdsink->fd, VIDIOC_REQBUFS, &req)) {
		if (EINVAL == errno) {
			fprintf(stderr, "%s does not support memory mapping\n", PATHDEVFD);
			return FALSE;
		} else {
			fprintf(stderr, "VIDIOC_REQBUFS\n");
			return FALSE;
		}
	}
	DBG("%s: req.count=%d\n", __func__, req.count);

	if (req.count < 1) {
		fprintf(stderr, "Insufficient buffer memory on %s\n", PATHDEVFD);
		return FALSE;
	}

	fdsink->buffers = calloc(req.count, sizeof(*(fdsink->buffers)));
	if (!fdsink->buffers) {
		fprintf(stderr, "Out of memory\n");
		return FALSE;
	}

	for(fdsink->n_buffers = 0; fdsink->n_buffers < req.count; ++(fdsink->n_buffers)) {
		struct v4l2_buffer buf;

		CLEAR(buf);

		buf.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = fdsink->n_buffers;

		if (-1 == xioctl(fdsink->fd, VIDIOC_QUERYBUF, &buf)) {
			fprintf(stderr, "VIDIOC_QUERYBUF\n");
			return FALSE;
		}

		DBG("%s: buf: idx=%d off=%x len=%d\n", "\t", buf.index, buf.m.offset, buf.length);

		fdsink->buffers[fdsink->n_buffers].length = buf.length;
		fdsink->buffers[fdsink->n_buffers].start = mmap(NULL /* start anywhere */, buf.length, PROT_READ | PROT_WRITE /* required */, MAP_SHARED /* recommended */, fdsink->fd, buf.m.offset);
		DBG("%s: buf: mmaped addr=%x\n", "\t", fdsink->buffers[fdsink->n_buffers]);

		if (MAP_FAILED == fdsink->buffers[fdsink->n_buffers].start) {
			fprintf(stderr, "mmap\n");
			return FALSE;
		}
	}
	DBG("\n");
	return TRUE;
}

static int init_device(struct GstFDSink *fdsink) {
	struct v4l2_capability cap;
	struct v4l2_format fmt;
	int ret = 0;

	if (-1 == xioctl(fdsink->fd, VIDIOC_QUERYCAP, &cap)) {
		if (EINVAL == errno) {
			fprintf(stderr, "%s is no V4L2 device\n", PATHDEVFD);
			return FALSE;
		} else {
			fprintf(stderr, "VIDIOC_QUERYCAP\n");
			return FALSE;
		}
	}

	dump_v4l2_cap(&cap, fdsink->output);

	if (!(cap.capabilities &(V4L2_CAP_VIDEO_OUTPUT | V4L2_CAP_OBJ_DETECTION))) {
		fprintf(stderr, "%s is not video output device or no object detection capability\n", PATHDEVFD);
		return FALSE;
	}

	if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
		fprintf(stderr, "%s does not support streaming i/o\n", PATHDEVFD);
		return FALSE;
	}

	CLEAR(fmt);
	fmt.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
	if (-1 == xioctl(fdsink->fd, VIDIOC_G_FMT, &fmt)) {
		fprintf(stderr, "VIDIOC_G_FMT\n");
		return FALSE;
	}

	fdsink->width = fmt.fmt.pix.width;
	fdsink->height = fmt.fmt.pix.height;
	dump_v4l2_format(&fmt, fdsink->output);

	return init_mmap(fdsink);
}

static void stop_detect(struct GstFDSink *fdsink) {
	enum v4l2_buf_type type;
	type = V4L2_BUF_TYPE_VIDEO_OUTPUT;

	if (-1 == xioctl(fdsink->fd, VIDIOC_STREAMOFF, &type)) {
		fprintf(stderr, "VIDIOC_STREAMOFF\n");
		return;
	}
}

static void start_detect(struct GstFDSink *fdsink) {
	unsigned int i;
	enum v4l2_buf_type type;

	for(i = 0; i < fdsink->n_buffers; ++i) {
		struct v4l2_buffer buf;

		CLEAR(buf);

		buf.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;

		if (-1 == xioctl(fdsink->fd, VIDIOC_QBUF, &buf)) {
			fprintf(stderr, "VIDIOC_QBUF\n");
			return;
		}
	}

	type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
	if (-1 == xioctl(fdsink->fd, VIDIOC_STREAMON, &type)) {
		fprintf(stderr, "VIDIOC_STREAMON\n");
		return;
	}
}

static int process_stream(struct GstFDSink *fdsink, struct v4l2_buffer *vbuf) {
	struct v4l2_od_count fc;
	struct v4l2_od_result *fr;

	fc.frm_seq = vbuf->sequence;
	if (-1 == xioctl(fdsink->fd, VIDIOC_G_OD_COUNT, &fc)) {
		switch(errno) {
		case EAGAIN:
			return 0;
		case EIO:
		default:
			fprintf(stderr, "VIDIOC_G_FD_COUNT\n");
			return -1;
		}
	}

	fr = malloc(fc.obj_cnt * sizeof(struct v4l2_od_object) + sizeof(*fr));

	fr->frm_seq = vbuf->sequence;
	fr->obj_cnt = fc.obj_cnt;
	if (-1 == xioctl(fdsink->fd, VIDIOC_G_OD_RESULT, fr)) {
		switch(errno) {
		case EAGAIN:
			return 0;
		case EIO:
		default:
			fprintf(stderr, "VIDIOC_G_FD_RESULT\n");
			return -1;
		}
	}

	dump_fdif_results(fr, fdsink->output, fdsink->factor);

	return 0;
}

static int handle_stream_detection(struct GstFDSink *fdsink) {
	struct v4l2_buffer buf;
	int ret = 1;

	CLEAR(buf);

	buf.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
	buf.memory = V4L2_MEMORY_MMAP;

	if (-1 == xioctl(fdsink->fd, VIDIOC_DQBUF, &buf)) {
		DBG("%s: errno=%d\n", __func__, errno);
		switch(errno) {
		case EAGAIN:
			return 0;
		case EIO:
		default:
			fprintf(stderr, "VIDIOC_DQBUF\n");
			return -1;
		}
	}

	assert(buf.index < fdsink->n_buffers);

	process_stream(fdsink, &buf);
	
	goto out;	

	if (-1 == xioctl(fdsink->fd, VIDIOC_QBUF, &buf)) {
		fprintf(stderr, "VIDIOC_QBUF\n");
		return -1;
	}

out:
	return ret;
}

#ifdef RESIZE_WITH_ARM
void resize_line(uint8_t * __restrict src1, uint8_t * __restrict src2, uint8_t * __restrict dest) {
	int w;
	for (w=0; w < 640; w += 16) {
		uint16x8_t a = vpaddlq_u8 (vld1q_u8 (src1));
		uint16x8_t b = vpaddlq_u8 (vld1q_u8 (src2));
		uint16x8_t c = vaddq_u16 (a,b);
		vst1_u8 (dest, vshrn_n_u16 (c, 2));
		src1+=16;
		src2+=16;
		dest+=8;
	}
}   
#endif

void resize_image(uint8_t * src, uint8_t * dest, int format169) {
	int h;
	int height = 240;
	uint8_t *dt = dest;

	if (format169) {
		height = 180;
		dt = dest + 320 * 30;
	}

	for (h = 0; h < height; h++) {
#ifdef RESIZE_WITH_ARM
		resize_line (src + 640 * (h * 2 + 0), src + 640 * (h * 2 + 1), dt + 320 * h);
#else
		int w;
		for (w = 0; w < 320; w++)
			dt[h * 320 + w] = (src[640 * h * 2 + w * 2] + src[640 * h * 2 + w * 2 + 1] + src[640 * h * 2 + 640 + w * 2] + src[640 * h * 2 + 640 + w * 2 + 1]) / 4;
#endif
	}
}

void faceDetect(uint8_t *dataIn, int flagsOutput, int width, int height) {
	int ret;

	fdsink->output = flagsOutput;
	if (flagsOutput == 0)
		return;

	start_detect(fdsink);

	if ((width == 320) && (height == 240))
		memcpy(fdsink->buffers[0].start, dataIn, fdsink->buffers[0].length);
	else
		resize_image(dataIn, (uint8_t *)(fdsink->buffers[0].start), (height == 360));

#ifdef STANDALONE
	FILE *f = fopen("out", "w+");
	if (f != NULL) {
		fwrite((uint8_t *)(fdsink->buffers[0].start), 1, 76800, f);
		fclose(f);
	}
#endif

	for(;;) {
		fd_set fds;
		struct timeval tv;

		FD_ZERO(&fds);
		FD_SET(fdsink->fd, &fds);

		/* Timeout. */
		tv.tv_sec = 1;
		tv.tv_usec = 0;

		ret = select(fdsink->fd + 1, &fds, NULL, NULL, &tv);

		if (-1 == ret) {
			if (EINTR == errno)
				continue;
			fprintf(stderr, "select failure\n");
			goto end;
		}

		if (0 == ret) {
			fprintf(stderr, "select timeout\n");
			goto end;
		}

		ret = handle_stream_detection(fdsink);
		if (ret >= 0)
			break;
		if (ret < 0)
			goto end;
	}

end:
	stop_detect(fdsink);
}

int faceDetectStart() {
	fdsink->factor = 4;
	fdsink->output = OUTPUT_PRINTF | OUTPUT_VIDEO | OUTPUT_MEMORY;

	if (!open_device(fdsink))
		return FALSE;

	if (!init_device(fdsink))
		return FALSE;

	return TRUE;
}

int faceDetectStop() {
	uninit_device(fdsink);

	close_device(fdsink);

	return TRUE;
}

#ifdef STANDALONE
int main(int argc, char** argv) {
	uint8_t inp[307200]; // 307200 = 640 * 480
	FILE *f = fopen("test", "r");
	if (f != NULL) {
		fread(inp, 1, 307200, f);
		faceDetectStart();
		faceDetect(inp, OUTPUT_PRINTF);
		faceDetectStop();
		fclose(f);
	}
}
#endif

