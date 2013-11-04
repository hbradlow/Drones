#include <cv.h>
#include <highgui.h>
#include <stdio.h>

#define CAM 1

float filteredX = 0;
float filteredY = 0;
float filtered_area = 0;

float alpha = .5;
float area_alpha = .1;

IplImage* GetSoftThresholdedImageRGB(IplImage* imgRGB){       
    IplImage* imgThresh=cvCreateImage(cvGetSize(imgRGB),IPL_DEPTH_8U, 1);
    cvInRangeS(imgRGB, cvScalar(150,150,150,0), cvScalar(256,256,256,0), imgThresh); 
    return imgThresh;
}
IplImage* GetThresholdedImageRGB(IplImage* imgRGB){       
    IplImage* imgThresh=cvCreateImage(cvGetSize(imgRGB),IPL_DEPTH_8U, 1);
    cvInRangeS(imgRGB, cvScalar(200,200,200,0), cvScalar(256,256,256,0), imgThresh); 
    return imgThresh;
}

void trackObject(IplImage* imgThresh){
    // Calculate the moments of 'imgThresh'
    CvMoments *moments = (CvMoments*)malloc(sizeof(CvMoments));
    cvMoments(imgThresh, moments, 1);
    double moment10 = cvGetSpatialMoment(moments, 1, 0);
    double moment01 = cvGetSpatialMoment(moments, 0, 1);
    double area = cvGetCentralMoment(moments, 0, 0);

    // if the area<1000, I consider that the there are no object in the image and it's because of the noise, the area is not zero 
    if(area>0){
        // calculate the position of the ball
        int posX = moment10/area;
        int posY = moment01/area;        
        printf("FOUND %d,%d\n",posX,posY);

        filteredX = alpha*posX + (1-alpha)*filteredX;
        filteredY = alpha*posY + (1-alpha)*filteredY;
        filtered_area = area*area_alpha + (1-area_alpha)*filtered_area;
        printf("AREA %f\n",filtered_area);
    }
    else{
        printf("Found nothing\n");
    }

    free(moments); 
}

int main(){
    int width = 320;
    int height = 240;

    uv_image uv_input_image, uv_output_image, uv_temp_image, uv_color1;
    uv_create_image(width, height, UV_GRAY, &uv_input_image);

    uv_camera_init("/dev/video6", width, height);

    uv_handle handle = NULL;
    uv_error result = uv_initialize(&handle, (unsigned char *)"licence.lic", (unsigned char *) "licence.sig", 0, NULL);

    while(1){
        int img_type =0; ///*** 0 -> gray image, 1-> RGB image
        uv_camera_capture(&uv_input_image, img_type);///*** 0 -> gray image, 1-> RGB image
        display(&uv_input_image); ///*** Need write permission to framebuffer0
/*
#if CAM
        IplImage *small = cvCloneImage(frame);
#else
        IplImage *small = cvCreateImage(
                            cvSize(cvGetSize(frame).width/8.0,
                                cvGetSize(frame).height/8.0),
                            IPL_DEPTH_8U, 
                            3);
        IplImage *tmp2 = cvCreateImage(
                            cvSize(cvGetSize(frame).width/4.0,
                                cvGetSize(frame).height/4.0),
                            IPL_DEPTH_8U, 
                            3);
        IplImage *tmp = cvCreateImage(
                            cvSize(cvGetSize(frame).width/2.0,
                                cvGetSize(frame).height/2.0),
                            IPL_DEPTH_8U, 
                            3);
        cvPyrDown(frame,tmp,CV_GAUSSIAN_5x5);
        cvPyrDown(tmp,tmp2,CV_GAUSSIAN_5x5);
        cvPyrDown(tmp2,small,CV_GAUSSIAN_5x5);
#endif

        IplImage *red = cvCreateImage(cvGetSize(small),IPL_DEPTH_8U, 1);
        IplImage *yuv = cvCreateImage(cvGetSize(small),IPL_DEPTH_8U, 3);
        IplImage *green = cvCreateImage(cvGetSize(small),IPL_DEPTH_8U, 1);
        IplImage *sub = cvCreateImage(cvGetSize(small),IPL_DEPTH_8U, 1);
        IplImage *sub_thresh = cvCreateImage(cvGetSize(small),IPL_DEPTH_8U, 1);
        IplImage *gray = cvCreateImage(cvGetSize(small),IPL_DEPTH_8U, 1);
        IplImage *thresh= cvCreateImage(cvGetSize(small),IPL_DEPTH_8U, 1);
        IplImage *green_thresh= cvCreateImage(cvGetSize(small),IPL_DEPTH_8U, 1);
        IplImage *solid= cvCreateImage(cvGetSize(small),IPL_DEPTH_8U, 1);


        cvCvtColor(small,yuv,CV_BGR2YCrCb);
        cvSplit(yuv,gray,green,red,NULL);
        cvAdd(red,green,red,NULL);

        cvSet(solid,cvScalar(255),NULL);
        cvSub(solid,red,sub,NULL);
        cvNormalize(sub,sub, 0, 255, CV_MINMAX, CV_8UC1);

        thresh = GetThresholdedImageRGB(sub);
        cvDilate(thresh,green_thresh,NULL,25);
        cvAnd(sub,green_thresh,green_thresh,NULL);
        green_thresh = GetSoftThresholdedImageRGB(green_thresh);

        cvDilate(green_thresh,green_thresh,NULL,5);
        cvErode(green_thresh,green_thresh,NULL,5);

        trackObject(green_thresh);

        cvRectangle(small,cvPoint(filteredX-10,filteredY-10),cvPoint(filteredX+10,filteredY+10),cvScalar(0xff,0x00,0x00,0x00),5,8,0);


        cvShowImage("Video", small);
        cvShowImage("Ball", thresh);
        cvShowImage("Green", green_thresh);
        cvShowImage("Sub", red);

        //Clean up used images
        cvReleaseImage(&frame);
        cvReleaseImage(&small);
        cvReleaseImage(&red);
        cvReleaseImage(&green);
        cvReleaseImage(&sub);
        cvReleaseImage(&sub_thresh);
        cvReleaseImage(&gray);
        cvReleaseImage(&thresh);
        cvReleaseImage(&green_thresh);

        //Wait 10mS
        int c = cvWaitKey(10);
        //If 'ESC' is pressed, break the loop
        if((char)c==27 ) break;      
        */
    }

    cvDestroyAllWindows() ;
    cvReleaseCapture(&capture);     

    return 0;
}
