#include <cv.h>
#include <highgui.h>
#include <stdio.h>

float filteredX = 0;
float filteredY = 0;

float alpha = .5;

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
    if(area>10){
        // calculate the position of the ball
        int posX = moment10/area;
        int posY = moment01/area;        
        printf("FOUND %d,%d\n",posX,posY);

        filteredX = alpha*posX + (1-alpha)*filteredX;
        filteredY = alpha*posY + (1-alpha)*filteredY;
    }
    else{
        printf("Found nothing\n");
    }

    free(moments); 
}

int main(){
    CvCapture* capture =0;       
    capture = cvCaptureFromCAM(0);
    if(!capture){
        printf("Capture failure\n");
        return -1;
    }

    IplImage* frame=0;
    frame = cvQueryFrame(capture);           

    if(!frame) return -1;

    cvNamedWindow("Video",0);     
    cvNamedWindow("Ball",0);

    while(1){
        frame = cvQueryFrame(capture);           

        if(!frame) break;
        frame=cvCloneImage(frame); 

        IplImage *small = cvCreateImage(
                            cvSize(cvGetSize(frame).width/2.0,
                                cvGetSize(frame).height/2.0),
                            IPL_DEPTH_8U, 
                            3);
        cvPyrDown(frame,small,CV_GAUSSIAN_5x5);

        IplImage *red = cvCreateImage(cvGetSize(small),IPL_DEPTH_8U, 1);
        IplImage *green = cvCreateImage(cvGetSize(small),IPL_DEPTH_8U, 1);
        IplImage *blue = cvCreateImage(cvGetSize(small),IPL_DEPTH_8U, 1);
        IplImage *sub = cvCreateImage(cvGetSize(small),IPL_DEPTH_8U, 1);
        IplImage *gray = cvCreateImage(cvGetSize(small),IPL_DEPTH_8U, 1);
        IplImage *thresh= cvCreateImage(cvGetSize(small),IPL_DEPTH_8U, 1);

        cvSplit(small,blue,green,red,NULL);
        cvCvtColor(small,gray,CV_BGR2GRAY);
        cvSub(green,gray,sub,NULL);
        cvNormalize(sub,sub, 0, 255, CV_MINMAX, CV_8UC1);

        thresh = GetThresholdedImageRGB(sub);
        trackObject(thresh);

        cvRectangle(small,cvPoint(filteredX-10,filteredY-10),cvPoint(filteredX+10,filteredY+10),cvScalar(0xff,0x00,0x00,0x00),5,8,0);

        cvShowImage("Video", small);
        cvShowImage("Ball", thresh);

        //Clean up used images
        cvReleaseImage(&frame);
        cvReleaseImage(&small);
        cvReleaseImage(&red);
        cvReleaseImage(&green);
        cvReleaseImage(&blue);
        cvReleaseImage(&sub);
        cvReleaseImage(&gray);
        cvReleaseImage(&thresh);

        //Wait 10mS
        int c = cvWaitKey(10);
        //If 'ESC' is pressed, break the loop
        if((char)c==27 ) break;      
    }

    cvDestroyAllWindows() ;
    cvReleaseCapture(&capture);     

    return 0;
}
