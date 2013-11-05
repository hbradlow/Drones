#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <time.h>
#include <linux/videodev2.h>
#include <highgui.h>
#include <opencv2/imgproc/types_c.h>
#include <uv_types.h>
#include <uv_v4l_camera_utils.h>
#include <uv_canny.h>
#include <uv_convolution_8u_3x3.h>
#include <uv_types1.h>
#include "ComWithMPU2.h"

#define ENGINE_NONE 0
#define ENGINE_CANNY 1
#define ENGINE_HOG 2
#define ENGINE_LKT 3
#define ENGINE_FACE 4
#define ENGINE_HEADBAND 5
#define OUTPUT_PRINTF 1
#define OUTPUT_VIDEO 2
#define OUTPUT_MEMORY 4

#define MAX_CORNERS 700

#define DATA(p, i) p.channel_data[i].pdata

uv_handle handle = NULL;
int input = 0;
int width = 0;
int height = 0;
int pixelFormat = 0;
IplImage *cvInputGrey;
uv_image uvInputGrey, uvInputNV12, uvPrevGrey;
char *bufCanny;
float deltaT = 0.0f;
int output = 0;

float deltaTime(struct timespec a, struct timespec b) {
	return (a.tv_nsec - b.tv_nsec) / 1000.0 / 1000.0 + ((a.tv_sec - b.tv_sec) * 1000.0);
}

void engineCanny() {
	char kernel[9] = {1, 2, 1, 2, 4, 2, 1, 2, 1};
	memcpy(uvPrevGrey.channel_data[0].pdata, uvInputGrey.channel_data[0].pdata, width * height);
	uv_convolution_8u_3x3(handle, &uvPrevGrey, &uvPrevGrey, kernel, 4);
	uv_canny(handle, &uvPrevGrey, &uvInputGrey, 10, 11, bufCanny);
}

void engineHOG() {
	int foundObjectCount = 0;
	uv_rect uvFound[200];
	struct uv_Size a;
	a.width = 8; a.height = 8;
	struct uv_Size b;
	b.width = 0; b.height = 0;
	uv_hogDetectMultiScale(handle, &uvInputGrey, uvFound, &foundObjectCount, 1.2f, a, b, 1.06, 2, 0);

	if (output & OUTPUT_PRINTF)
		printf("Pedestrians found: %d\n",foundObjectCount);
	int c = 0;
	for(c = 0; c < foundObjectCount; c++) {
		UvRect r = uvFound[c];
		uv_drawRectangle(&cvInputGrey, r, uvRealScalar(255), 0);
		if (output & OUTPUT_PRINTF)
			printf("UV\t(%d, %d)\t-->\t(%d, %d)\n", r.x, r.y, r.x + r.width, r.y + r.height);
	}
}

void engineLKT() {
	uv_Point32f vector;
	int number_of_features = 0;
	int number_of_vectors = 0;
	uv_Point32f frame1_features[MAX_CORNERS];
	uv_Point32f frame2_features[MAX_CORNERS];
	vector.x = 0.0f;
	vector.y = 0.0f;
	char optical_flow_found_feature[MAX_CORNERS];
	float optical_flow_feature_error[MAX_CORNERS], quality_level = 0.04;
	int level = 4, min_distance = 3;
	uv_Size windowSize;
	windowSize.width = windowSize.height = 7;

	uv_TermCriteria termination_criteria;
	termination_criteria.type = 3;
	termination_criteria.max_iter = 20;
	termination_criteria.epsilon = 0.3;

	uv_image32 harris_image_out;
	uv_create_image32(width, height, UV_GRAY, &harris_image_out);

	uv_image pyramid1, pyramid2;
	uv_create_image(width, height, UV_GRAY, &pyramid1);
	uv_create_image(width, height, UV_GRAY, &pyramid2);

	number_of_features = MAX_CORNERS;
	uv_goodfeaturesToTrack(handle, &uvPrevGrey, &harris_image_out, frame1_features, &number_of_features, &quality_level, &min_distance);

	uv_optical_flow(handle, &uvPrevGrey, &uvInputGrey, &pyramid1, &pyramid2, frame1_features, frame2_features, number_of_features, optical_flow_found_feature, optical_flow_feature_error, level, termination_criteria);

	memcpy(uvPrevGrey.channel_data[0].pdata, uvInputGrey.channel_data[0].pdata, width * height);

	int c = 0;
	number_of_vectors = 0;
	for (c = 0; c < number_of_features; c++) {
		if (optical_flow_found_feature[c] == 0)
			continue;
		uv_Point32f v;
		v.x = frame2_features[c].x - frame1_features[c].x;
		v.y = frame2_features[c].y - frame1_features[c].y;
		if (v.x * v.x + v.y * v.y < 50.0f) {
			number_of_vectors++;
			vector.x += v.x;
			vector.y += v.y;
			if (output & OUTPUT_PRINTF)
				uv_drawLine(&uvInputGrey, uvPoint(round(frame1_features[c].x), round(frame1_features[c].y)), uvPoint(round(frame2_features[c].x),round(frame2_features[c].y)), uvRealScalar(255), 0);
		}
	}
	if (number_of_vectors != 0.0f) {
		vector.x /= number_of_vectors;
		vector.y /= number_of_vectors;
	}

	if (output & OUTPUT_MEMORY) {
		struct structCVtoMPU2 data;
		data.tripodvx = vector.x;
		data.tripodvx = vector.y;
		data.type = TYPE_PILOT_TRIPOD;
		CVtoMPU2_Send(input <= 1, &data);
	}

	if (output & OUTPUT_PRINTF)
		printf("%+6.1f %+6.1f f: %3d/%3d t: %5.1f\n", vector.x * 5.0f, vector.y * 5.0f, number_of_vectors, number_of_features, deltaT);
}

void engineHeadband() {
	//cvInputGrey has the grey image
	//uvInputGrey has the grey image - cvInputGrey and uvInputGrey share the same memory region
	//uvInputNV12 has the NV12 image - the first plane, cvInputGrey and uvInputGrey share the same memory region
	//For 1 NV12 pixel: YYYYYYYY UVUV
	//For a 2-pixel NV12 frame: YYYYYYYYYYYYYYYY UVUVUVUV

	//Work only on uvInputGrey or on uvInputNV12.channel_data[1].pdata
	//Don't conver to RGB
	//Minimize the number of images and operations

	uv_image add,thresh,u,v;
	int momentX,momentY,area;
	uv_create_image(width, height, UV_GRAY, &u);
	uv_create_image(width, height, UV_GRAY, &v);
	uv_create_image(width, height, UV_GRAY, &add);
	uv_create_image(width, height, UV_GRAY, &thresh);
	extractNV12toUV(DATA(uvInputNV12,1),DATA(u,0),DATA(v,0),DATA(add,0),DATA(thresh,0),cvInputGrey->imageData, width,height,&momentX,&momentY,&area);

	//cvInputGrey->imageData = add.channel_data[0].pdata;

	if (output & OUTPUT_VIDEO) {
#if 1
		//To show in black&white, uncomment this:
		cvShowImage("OUTPUT WINDOW", cvInputGrey);
#else
		//To show in color, uncomment this:
		IplImage *cvRGB = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);

		convertNV12toRGB(uvInputNV12.channel_data[0].pdata, uvInputNV12.channel_data[1].pdata, cvRGB->imageData, width, height);
		//convertNV12toRGB(add.channel_data[0].pdata, add.channel_data[1].pdata, cvRGB->imageData, width, height);

		cvShowImage("OUTPUT WINDOW", cvRGB);
		cvReleaseImage(&cvRGB);
#endif
	}

	if (output & OUTPUT_MEMORY) {
		struct structCVtoMPU2 data;
		data.headbandx = momentX;
		data.headbandy = momentY;
		data.headbandarea = area;
		data.type = TYPE_PILOT_HEADBAND;
		CVtoMPU2_Send(input <= 1, &data);
	}
}

int main(int argc, char** argv) {
	int engine = 0;
	int option = 0;

//	setpriority(PRIO_PROCESS, 0, -20);
//	cpu_set_t mask;
//	CPU_ZERO(&mask);
//	CPU_SET(0, &mask);
//	sched_setaffinity(0, sizeof(mask), &mask);

	input = 1;
	engine = ENGINE_LKT;
	output = OUTPUT_PRINTF + OUTPUT_VIDEO + OUTPUT_MEMORY;
	width = 320;
	height = 240;

	while ((option = getopt(argc, argv, "d:e:ho:u:v:")) != -1) {
		switch (option) {
		case 'd':
			sscanf(optarg, "%d", &input);
			break;
		case 'e':
			sscanf(optarg, "%d", &engine);
			break;
		case 'h':
			printf("*******************************************************\n");
			printf("Usage for aicv [-d input -e engine -h -o output -u width -v height]\n");
			printf("d input:	Select input. csi2b-NV12: 0, csi2b-Y8: 1, webcam-YUYV: 2 (default: 1)\n");
			printf("e engine:	Select engine. none: 0, canny: 1, hog: 2, lkt: 3, face: 4, headband: 5 (default: 3)\n");
			printf("h:		Print this usage and exit\n");
			printf("o output:	Select output. printf:1, video:2, memory:4 (default: 7)\n");
			printf("u width:	Set width (default: 320)\n");
			printf("v height:	Set height (default: 240)\n");
			exit(0);
			break;
		case 'o':
			sscanf(optarg, "%d", &output);
			break;
		case 'u':
			sscanf(optarg, "%d", &width);
			break;
		case 'v':
			sscanf(optarg, "%d", &height);
			break;
		default:
			break;
		}
	}

	if (output & OUTPUT_MEMORY)
		InitComWithMPU2(input);

	if (input == 0) {
		pixelFormat = V4L2_PIX_FMT_NV12;
		uv_camera_init("/dev/video5", width, height, pixelFormat);
	} else if (input == 1) {
		pixelFormat = V4L2_PIX_FMT_GREY;
		uv_camera_init("/dev/video5", width, height, pixelFormat);
	} else if (input == 2) {
		pixelFormat = V4L2_PIX_FMT_YUYV;
		uv_camera_init("/dev/video6", width, height, pixelFormat);
	}

	if (output & OUTPUT_VIDEO)
		cvNamedWindow("OUTPUT WINDOW", 1);

	cvInputGrey = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	uvInputGrey.width = width;
	uvInputGrey.height = height;
	uvInputGrey.channel_data[0].stride = width;
	uvInputGrey.channel_data[0].pdata = cvInputGrey->imageData;

	uv_create_image(width, height, UV_YUV_PLANAR, &uvInputNV12);
	uvInputNV12.channel_data[0].stride = width;
	uvInputNV12.channel_data[0].pdata = cvInputGrey->imageData;
	uvInputNV12.channel_data[1].stride = width / 2;

	uv_create_image(width, height, UV_GRAY, &uvPrevGrey);

	uv_error result = uv_initialize(&handle, (unsigned char *)"/home/ai/aiCV/licence.lic", (unsigned char *) "/home/ai/aiCV/licence.sig", 0, NULL);
	if(result != UV_E_SUCCESS)
		return result;

	uv_init_canny_buf(&uvPrevGrey, &bufCanny);
	faceDetectStart();

	struct timespec lts;
	while (1) {
		struct timespec ts;
		clock_gettime(CLOCK_REALTIME, &ts);
		deltaT = deltaTime(ts, lts);
		lts = ts;
		printf("Time %f\n",1000.0f/deltaT);

		if (engine == ENGINE_HEADBAND)
			uv_camera_capture(0, &uvInputNV12, pixelFormat);
		else
			uv_camera_capture(1, &uvInputGrey, pixelFormat);

		switch(engine) {
		case ENGINE_NONE:
			break;
		case ENGINE_CANNY:
			engineCanny();
			break;
		case ENGINE_HOG:
			engineHOG();
			break;
		case ENGINE_LKT:
			engineLKT();
			break;
		case ENGINE_FACE:
			faceDetect(uvInputGrey.channel_data[0].pdata, output, width, height);
			break;
		case ENGINE_HEADBAND:
			engineHeadband();
			break;
		default:
			break;
		}

		if (output & OUTPUT_VIDEO) {
			if (engine != ENGINE_HEADBAND) //Headband is doing its own rendering
				cvShowImage("OUTPUT WINDOW", cvInputGrey);
			if(cvWaitKey(1) == 27)
				break;
		}
	}

	faceDetectStop();
	uv_free_canny_buf(bufCanny);
	uv_camera_close();
	uv_deinitialize(handle);
	cvReleaseImage(&cvInputGrey);
	if (output & OUTPUT_VIDEO)
		cvDestroyWindow("OUTPUT WINDOW");
	if (output & OUTPUT_MEMORY)
		UninitComWithMPU2(input);
	return 0;
}

