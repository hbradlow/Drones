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
    if(area>100){
        // calculate the position of the ball
        int posX = moment10/area;
        int posY = moment01/area;        

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
    {
        //printf("Found nothing\n");
    }

    printf("FOUND %f,%f\n",filteredX,filteredY);
     free(moments); 
}


int main(){
   
      CvCapture* capture =0;       
      capture = cvCaptureFromCAM(0);
      if(!capture){
         printf("Capture failure\n");
         return -1;
      }
      
      IplImage* tmp=0;
      tmp = cvQueryFrame(capture);           
      IplImage* tmp2=cvCreateImage(cvSize(cvGetSize(tmp).width/2.0,cvGetSize(tmp).height/2.0),IPL_DEPTH_8U, 3);
      IplImage* frame=cvCreateImage(cvSize(cvGetSize(tmp).width/4.0,cvGetSize(tmp).height/4.0),IPL_DEPTH_8U, 3);

      cvPyrDown(tmp,tmp2,CV_GAUSSIAN_5x5);
      cvPyrDown(tmp2,frame,CV_GAUSSIAN_5x5);
      if(!frame) return -1;
   
     //create a blank image and assigned to 'imgTracking' which has the same size of original video
     imgTracking=cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U, 3);
     cvZero(imgTracking); //covert the image, 'imgTracking' to black

     cvNamedWindow("Video",0);     
     cvNamedWindow("Ball",0);

      //iterate through each frames of the video     
      while(1){

            frame = cvQueryFrame(capture);           

          tmp = cvQueryFrame(capture);           
          tmp2=cvCreateImage(cvSize(cvGetSize(tmp).width/2.0,cvGetSize(tmp).height/2.0),IPL_DEPTH_8U, 3);
          frame=cvCreateImage(cvSize(cvGetSize(tmp).width/4.0,cvGetSize(tmp).height/4.0),IPL_DEPTH_8U, 3);

          cvPyrDown(tmp,tmp2,CV_GAUSSIAN_5x5);
          cvPyrDown(tmp2,frame,CV_GAUSSIAN_5x5);

            if(!frame) break;
            frame=cvCloneImage(frame); 
            
            //cvSmooth(frame, frame, CV_GAUSSIAN,3,3); //smooth the original image using Gaussian kernel

            IplImage* imgRGB = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3); 
            IplImage* imgGray = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1); 
            IplImage* imgSub = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1); 
            IplImage* imgRed = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1); 

            cvCvtColor(frame, imgRGB, CV_BGR2RGB); //Change the color format from BGR to HSV
            cvCvtColor(frame, imgGray, CV_BGR2GRAY);

            cvSplit(imgRGB,imgRed,NULL,NULL,NULL);

            cvSub(imgRed,imgGray,imgSub,NULL);
            cvNormalize(imgSub,imgSub,0,255,CV_MINMAX,NULL);

            IplImage* imgThreshSub = GetThresholdedImageRGB(imgSub);

            trackObject(imgThreshSub);
          
            

            // Add the tracking image and the frame
           //cvAdd(frame, imgTracking, frame, NULL);

           cvShowImage("Video", frame);
           cvShowImage("Ball", imgThreshSub);
           
           //Clean up used images
           cvReleaseImage(&imgRGB);
           cvReleaseImage(&imgThreshSub);            
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
