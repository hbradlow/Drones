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
    CvCapture* capture =0;       
#if CAM
    capture = cvCaptureFromCAM(0);
#else
    capture = cvCaptureFromFile("../data/IMG_5967.MOV");
#endif
    cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH, 320);
    cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT, 240);
    if(!capture){
        printf("Capture failure\n");
        return -1;
    }

    IplImage* frame=0;
    frame = cvQueryFrame(capture);           

    if(!frame) return -1;

    cvNamedWindow("Video",0);     
    cvNamedWindow("Ball",0);
    cvNamedWindow("Green",0);
    cvNamedWindow("Sub",0);

    while(1){
        frame = cvQueryFrame(capture);           

        if(!frame) break;
        frame=cvCloneImage(frame); 

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
    }

    cvDestroyAllWindows() ;
    cvReleaseCapture(&capture);     

    return 0;
}
