#include <uv_types.h>
#include <uv_errors.h>
#include <helpers.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <asm/types.h>
#include <linux/videodev2.h>
#include <time.h>

#define CLEAR(x) memset(&(x), 0, sizeof(x))
int CAM_WIDTH;
int CAM_HEIGHT;

typedef struct buffer {
	void *start;
	size_t length;
} buffer;

char *dev_name = NULL;
int fd = -1;
buffer *buffers = NULL;
unsigned int n_buffers = 0;

void errno_exit(const char *s) {
	fprintf(stderr, "%s error %d, %s\n", s, errno, strerror(errno));
	exit(EXIT_FAILURE);
}

int xioctl(int fd, int request, void *arg) {
	int r;

	do r = ioctl(fd, request, arg) ;
	while (-1 == r && EINTR == errno) ;
	return r;
}

int clamp(double x) {
	int r = x;      /* round to nearest */

	if (r < 0)         return 0;
	else if (r > 255)  return 255;
	else               return r;
}

void yuv420_rgb(unsigned char Y1, unsigned char Cb, unsigned char Cr, char *ER, char *EG, char *EB) {
	double r, g, b;
	double y1, pb, pr;

	y1 =(255 / 219.0) *(Y1 - 16);
	pb =(255 / 224.0) *(Cb - 128);
	pr =(255 / 224.0) *(Cr - 128);

	r = 1.0 * y1 + 0     * pb + 1.402 * pr;
	g = 1.0 * y1 - 0.344 * pb - 0.714 * pr;
	b = 1.0 * y1 + 1.772 * pb + 0     * pr;

	*ER = clamp(r); /* [ok? one should prob. limit y1,pb,pr] */
	*EG = clamp(g);
	*EB = clamp(b);
}

void convertNV12toRGB(char *y, char *uv, char *rgb, int w, int h) {
	int i, j;
	for (j = 0; j < h; j++)
		for (i = 0; i < w; i++) {
			char r, g, b;
			char y_val, u, v;
			y_val = y[j * w + i];
			u = uv[((int)(j / 2)) * w + ((int)(i / 2)) * 2];
			v = uv[((int)(j / 2)) * w + ((int)(i / 2)) * 2 + 1];
			yuv420_rgb(y_val, u, v, &r, &g, &b);
			rgb[(w * j * 3) + i * 3] = (int)b;
			rgb[(w * j * 3) + i * 3 + 1] = (int)g;
			rgb[(w * j * 3) + i * 3 + 2] = (int)r;
		}
}

void convertYUYVtoY8(char *src, char *dest, int w, int h) {
	int i, j;
	char *y, *gray;
	//get only Y component for grayscale from(Y1)(U1,2)(Y2)(V1,2)
	for (j = 0; j < h; j++) {
		y = src + (w * 2 * j);
		gray = dest + (w * j);
		for (i = 0; i < w; i += 2) {
			gray[i] = *y;
			y += 2;
			gray[i + 1] = *y;
			y += 2;
		}
	}
}

void Extract02_NEON(const char *src_uyvy, char *a, int w) {
#ifdef ARM
	asm volatile(
		"1:                                      \n"
		"vld4.u8    {d0,d1,d2,d3}, [%0]!        \n" // load 16 pairs of UYVY
		"subs  %2, %2, #16                     \n" // 16 processed per loop
		"vst2.u8   {d0,d2}, [%1]!              \n" // store back 0 and 2
		"bgt   1b                              \n" // Loop back if not done
		: "+r"(src_uyvy), // %0
		"+r"(a), // %1
		"+r"(w)     // %3      // output registers
		:                            // input registers
		: "memory", "cc", "q0", "q1", "q2", "q3" // Clobber List
	);                             
#endif
}

void Extract13_NEON(const char *src_uyvy, char *a, int w) {
#ifdef ARM
	asm volatile(
		"1:                                      \n"
		"vld4.u8    {d0,d1,d2,d3}, [%0]!        \n" // load 16 pairs of UYVY
		"subs  %2, %2, #16                     \n" // 16 processed per loop
		"vst2.u8   {d1,d3}, [%1]!              \n" // store back 1 and 3
		"bgt   1b                              \n" // Loop back if not done
		: "+r"(src_uyvy), // %0
		"+r"(a), // %1
		"+r"(w)     // %3      // output registers
		:                            // input registers
		: "memory", "cc", "q0", "q1", "q2", "q3" // Clobber List
	);                            
#endif
}

void convertYUYVtoNV12(char *src, char *destY, char *destUV, int w, int h) {
	int i;
	for (i = 0; i < h; i += 2) {
		Extract13_NEON(src, destUV, w);
		destUV += w;
		Extract02_NEON(src, destY, w);
		destY += w;
		src += w * 2;
		Extract02_NEON(src, destY, w);
		destY += w;
		src += w * 2;
	}
}

int read_frame(int greyWanted, uv_image *captured_img, int pixeFormat) {
	char *img_ptr;
	struct v4l2_buffer buf;
	unsigned int i;

	img_ptr = captured_img->channel_data[0].pdata;

	CLEAR(buf);
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;

	if (-1 == xioctl(fd, VIDIOC_DQBUF, &buf)) {
		switch(errno) {
		case EAGAIN:
			return 0;

		case EIO:
			/* Could ignore EIO, see spec. */

			/* fall through */

		default:
			errno_exit("VIDIOC_DQBUF");
		}
	}

	assert(buf.index < n_buffers);

	if (pixeFormat == V4L2_PIX_FMT_GREY) {
		memcpy(img_ptr, buffers[buf.index].start, buffers[buf.index].length);
		if (!greyWanted)
			memset(img_ptr + buffers[buf.index].length, 0x80, buffers[buf.index].length / 2);
	} else if (pixeFormat == V4L2_PIX_FMT_YUYV) {
		if (greyWanted)
			convertYUYVtoY8(buffers[buf.index].start, img_ptr, CAM_WIDTH, CAM_HEIGHT);
		else
			convertYUYVtoNV12(buffers[buf.index].start, img_ptr, captured_img->channel_data[1].pdata, CAM_WIDTH, CAM_HEIGHT);
	} else if (pixeFormat == V4L2_PIX_FMT_NV12) {
		if (greyWanted)
			memcpy(img_ptr, buffers[buf.index].start, buffers[buf.index].length * 2 / 3);
		else {
			memcpy(img_ptr, buffers[buf.index].start, buffers[buf.index].length * 2 / 3);
			memcpy(captured_img->channel_data[1].pdata, buffers[buf.index].start + buffers[buf.index].length * 2 / 3, buffers[buf.index].length / 3);
		}
	} 

	if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
		errno_exit("VIDIOC_QBUF");

	return 1;
}

void stop_capturing(void) {
	enum v4l2_buf_type type;

	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if (-1 == xioctl(fd, VIDIOC_STREAMOFF, &type))
		errno_exit("VIDIOC_STREAMOFF");
}

void start_capturing(void) {
	unsigned int i;
	enum v4l2_buf_type type;

	for (i = 0; i < n_buffers; ++i) {
		struct v4l2_buffer buf;
		CLEAR(buf);
		buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory      = V4L2_MEMORY_MMAP;
		buf.index       = i;
		if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
			errno_exit("VIDIOC_QBUF");
	}
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if (-1 == xioctl(fd, VIDIOC_STREAMON, &type))
		errno_exit("VIDIOC_STREAMON");

}

void uninit_device(void) {
	unsigned int i;
	for (i = 0; i < n_buffers; ++i)
		if (-1 == munmap(buffers[i].start, buffers[i].length))
			errno_exit("munmap");
	free(buffers);
}

void init_read(unsigned int buffer_size) {
	buffers =(buffer*)calloc(1, sizeof(*buffers));

	if (!buffers) {
		fprintf(stderr, "Out of memory\n");
		exit(EXIT_FAILURE);
	}

	buffers[0].length = buffer_size;
	buffers[0].start = malloc(buffer_size);

	if (!buffers[0].start) {
		fprintf(stderr, "Out of memory\n");
		exit(EXIT_FAILURE);
	}
}

void init_mmap(void) {
	struct v4l2_requestbuffers req;

	CLEAR(req);
	req.count               = 4;
	req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory              = V4L2_MEMORY_MMAP;

	if (-1 == xioctl(fd, VIDIOC_REQBUFS, &req)) {
		if (EINVAL == errno) {
			fprintf(stderr, "%s does not support memory mapping\n", dev_name);
			exit(EXIT_FAILURE);
		} else
			errno_exit("VIDIOC_REQBUFS");
	}

	if (req.count < 2) {
		fprintf(stderr, "Insufficient buffer memory on %s\n", dev_name);
		exit(EXIT_FAILURE);
	}

	buffers =(buffer*)calloc(req.count, sizeof(*buffers));
	if (!buffers) {
		fprintf(stderr, "Out of memory\n");
		exit(EXIT_FAILURE);
	}

	for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
		struct v4l2_buffer buf;
		CLEAR(buf);
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = n_buffers;

		if (-1 == xioctl(fd, VIDIOC_QUERYBUF, &buf))
			errno_exit("VIDIOC_QUERYBUF");

		buffers[n_buffers].length = buf.length;
		buffers[n_buffers].start = mmap(NULL /* start anywhere */, buf.length, PROT_READ | PROT_WRITE /* required */, MAP_SHARED /* recommended */, fd, buf.m.offset);

		if (MAP_FAILED == buffers[n_buffers].start)
			errno_exit("mmap");
	}
}

void init_device(int w, int h, int pixelFormat) {
	struct v4l2_capability cap;
	struct v4l2_cropcap cropcap;
	struct v4l2_crop crop;
	struct v4l2_format fmt;
	unsigned int min;

	if (-1 == xioctl(fd, VIDIOC_QUERYCAP, &cap)) {
		if (EINVAL == errno) {
			fprintf(stderr, "%s is no V4L2 device\n", dev_name);
			exit(EXIT_FAILURE);
		} else
		errno_exit("VIDIOC_QUERYCAP");
	}

	if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
		fprintf(stderr, "%s is no video capture device\n", dev_name);
		exit(EXIT_FAILURE);
	}

	if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
		fprintf(stderr, "%s does not support streaming i/o\n", dev_name);
		exit(EXIT_FAILURE);
	}

	CLEAR(fmt);
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width = w;
	fmt.fmt.pix.height = h;
	fmt.fmt.pix.pixelformat =pixelFormat;
	fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;

	if (-1 == xioctl(fd, VIDIOC_S_FMT, &fmt))
		errno_exit("VIDIOC_S_FMT");

	/* Note VIDIOC_S_FMT may change width and height. */

	/* Buggy driver paranoia. */
	min = fmt.fmt.pix.width * 2;
	if (fmt.fmt.pix.bytesperline < min)
		fmt.fmt.pix.bytesperline = min;
	min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
	if (fmt.fmt.pix.sizeimage < min)
		fmt.fmt.pix.sizeimage = min;
	init_mmap();
}

void close_device(void) {
	if (-1 == close(fd))
		errno_exit("close");
	fd = -1;
}

void open_device(void) {
	struct stat st;

	if (-1 == stat(dev_name, &st)) {
		fprintf(stderr, "Cannot identify '%s': %d, %s\n", dev_name, errno, strerror(errno));
		exit(EXIT_FAILURE);
	}

	if (!S_ISCHR(st.st_mode)) {
		fprintf(stderr, "%s is no device\n", dev_name);
		exit(EXIT_FAILURE);
	}

	fd = open(dev_name, O_RDWR /* required */ | O_NONBLOCK, 0);
	if (-1 == fd) {
		fprintf(stderr, "Cannot open '%s': %d, %s\n", dev_name, errno, strerror(errno));
		exit(EXIT_FAILURE);
	}
}

uv_error uv_camera_init(char *dev_str, int w, int h, int pixelFormat) {
	dev_name = dev_str;
	CAM_WIDTH = w;
	CAM_HEIGHT = h;
	open_device();
	init_device(w, h, pixelFormat);
	start_capturing();
	return UV_E_SUCCESS;
}

uv_error uv_camera_capture(int greyWanted, uv_image *captured_img, int pixelFormat) {
	for (;;) {
		fd_set fds;
		struct timeval tv;
		int r;

		FD_ZERO(&fds);
		FD_SET(fd, &fds);

		/* Timeout. */
		tv.tv_sec = 2;
		tv.tv_usec = 0;

		r = select(fd + 1, &fds, NULL, NULL, &tv);

		if (-1 == r) {
			if (EINTR == errno)
				continue;
			errno_exit("select");
		}

		if (0 == r) {
			fprintf(stderr, "select timeout\n");
			exit(EXIT_FAILURE);
		}

		if (read_frame(greyWanted, captured_img, pixelFormat))
			break;
		/* EAGAIN - continue select loop. */
	}
}

uv_error uv_camera_close() {
	stop_capturing();
	uninit_device();
	close_device();
	return 0;
}

