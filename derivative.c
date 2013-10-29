#include <cv.h>
#include <highgui.h>
#include <stdio.h>

IplImage* imgTracking;
int lastX = -1;
int lastY = -1;

float filteredX = 0;
float filteredY = 0;

float alpha = .5;

//This function threshold the HSV image and create a binary image
IplImage* GetThresholdedImageRGB(IplImage* imgRGB){       
    IplImage* imgThresh=cvCreateImage(cvGetSize(imgRGB),IPL_DEPTH_8U, 1);
    cvInRangeS(imgRGB, cvScalar(200,200,200,0), cvScalar(256,256,256,0), imgThresh); 
    return imgThresh;
}

int main(){
   
      CvCapture* capture =0;       
      capture = cvCaptureFromCAM(0);
      if(!capture){
         printf("Capture failure\n");
         return -1;
      }
      
      IplImage* frame=0;
      IplImage* previous=0;
      IplImage* onCount=0;
      frame = cvQueryFrame(capture);           
      previous = cvCloneImage(frame);

      if(!frame) return -1;
   
     //create a blank image and assigned to 'imgTracking' which has the same size of original video
     imgTracking=cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U, 3);
     cvZero(imgTracking); //covert the image, 'imgTracking' to black

     onCount=cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U, 1);
     cvZero(onCount); //covert the image, 'imgTracking' to black

     cvNamedWindow("Video",0);     
     cvNamedWindow("Ball",0);

      //iterate through each frames of the video     
      while(1){

            frame = cvQueryFrame(capture);           

            if(!frame) break;
            frame=cvCloneImage(frame); 

            IplImage* sub = cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U,3);
            IplImage* on = GetThresholdedImageRGB(frame);

            cvAddWeighted(on,alpha,onCount,1-alpha,0,onCount);

            cvSub(frame,previous,sub,NULL);

           cvShowImage("Video", frame);
           cvShowImage("Ball", onCount);
           
            previous = cvCloneImage(frame);

           //Clean up used images
           cvReleaseImage(&frame);

            //Wait 10mS
            int c = cvWaitKey(10);
            //If 'ESC' is pressed, break the loop
            if((char)c==27 ) break;      
      }

      cvDestroyAllWindows() ;
      cvReleaseImage(&imgTracking);
      cvReleaseCapture(&capture);     

      return 0;
}
