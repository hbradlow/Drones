#include <cv.h>
#include <highgui.h>
#include <stdio.h>

IplImage* imgTracking;
int lastX = -1;
int lastY = -1;

float filteredX = 0;
float filteredY = 0;
float alpha = .2;

//This function threshold the HSV image and create a binary image
IplImage* GetThresholdedImageRGB(IplImage* imgRGB){       
    IplImage* imgThresh=cvCreateImage(cvGetSize(imgRGB),IPL_DEPTH_8U, 1);
    cvInRangeS(imgRGB, cvScalar(200,200,200,0), cvScalar(256,256,256,0), imgThresh); 
    return imgThresh;
}
IplImage* GetThresholdedImageHSV(IplImage* imgHSV){       
    IplImage* imgThresh=cvCreateImage(cvGetSize(imgHSV),IPL_DEPTH_8U, 1);
    cvInRangeS(imgHSV, cvScalar(170,160,60,0), cvScalar(180,256,256,0), imgThresh); 
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
    if(area>1000){
        // calculate the position of the ball
        int posX = moment10/area;
        int posY = moment01/area;        
        printf("FOUND %d,%d\n",posX,posY);

        filteredX = alpha*posX + (1-alpha)*filteredX;
        filteredY = alpha*posY + (1-alpha)*filteredY;
        
       if(lastX>=0 && lastY>=0 && posX>=0 && posY>=0)
        {
            // Draw a yellow line from the previous point to the current point
            cvLine(imgTracking, cvPoint(posX, posY), cvPoint(lastX, lastY), cvScalar(0,0,255,0), 4,5,5);
        }

         lastX = posX;
        lastY = posY;
    }
    else
        printf("Found nothing\n");

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
   
     //create a blank image and assigned to 'imgTracking' which has the same size of original video
     imgTracking=cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U, 3);
     cvZero(imgTracking); //covert the image, 'imgTracking' to black

     cvNamedWindow("Video",0);     
     cvNamedWindow("Ball",0);

      //iterate through each frames of the video     
      while(1){

            frame = cvQueryFrame(capture);           
            if(!frame) break;
            frame=cvCloneImage(frame); 
            
            //cvSmooth(frame, frame, CV_GAUSSIAN,3,3); //smooth the original image using Gaussian kernel

            IplImage* imgHSV = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3); 
            IplImage* imgRGB = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3); 
            cvCvtColor(frame, imgHSV, CV_BGR2HSV); //Change the color format from BGR to HSV
            cvCvtColor(frame, imgRGB, CV_BGR2RGB); //Change the color format from BGR to HSV
            IplImage* imgThreshHSV = GetThresholdedImageHSV(imgHSV);
            IplImage* imgThreshRGB = GetThresholdedImageRGB(imgRGB);
          
            //cvSmooth(imgThreshHSV, imgThreshHSV, CV_GAUSSIAN,3,3,3,3); //smooth the binary image using Gaussian kernel
            cvSmooth(imgThreshRGB, imgThreshRGB, CV_GAUSSIAN,3,3,3,3); //smooth the binary image using Gaussian kernel

            cvDilate(imgThreshHSV, imgThreshHSV, NULL, 10);
            
           //track the possition of the ball
           trackObject(imgThreshHSV);

            // Add the tracking image and the frame
           cvAdd(frame, imgTracking, frame, NULL);

            cvShowImage("Ball", imgThreshHSV);           
            cvShowImage("Ball2", imgThreshRGB);           
           cvShowImage("Video", frame);
           
           //Clean up used images
           cvReleaseImage(&imgHSV);
           cvReleaseImage(&imgRGB);
           cvReleaseImage(&imgThreshHSV);            
           cvReleaseImage(&imgThreshRGB);            
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
