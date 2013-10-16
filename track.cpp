#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <ctime>
#include <sys/time.h>

using namespace cv;
using namespace std;

long get_mtime(){
    struct timeval start;
    gettimeofday(&start, NULL);
    long mtime = ((start.tv_sec) * 1000 + start.tv_usec/1000.0) + 0.5;
    return mtime;
}

class Ticker{
    float alpha;
    float frequency;
    int count;
    int print_period;
    time_t previous_time;
public:
    Ticker(){
        this->alpha = .5;
        this->frequency = 0;
        this->count = 0;
        this->previous_time = get_mtime();
        this->print_period = 20;
    }
    void tick(){

        this->count += 1;
        double now = get_mtime();
        float diff = (now-this->previous_time)/1000.0;
        this->previous_time = now;

        if(diff==0)
            return;

        float new_frequency = 1.0/diff;
        this->frequency = this->alpha*new_frequency 
                            + (1-this->alpha)*this->frequency;

        if(this->count % this->print_period == 0)
            printf("Frequency %f Hz\n",this->frequency);
    }
};

void FindBlobs(const cv::Mat &binary, std::vector < std::vector<cv::Point2i> > &blobs)
{
    blobs.clear();

    // Fill the label_image with the blobs
    // 0  - background
    // 1  - unlabelled foreground
    // 2+ - labelled foreground

    cv::Mat label_image;
    binary.convertTo(label_image, CV_32SC1);

    int label_count = 2; // starts at 2 because 0,1 are used already

    for(int y=0; y < label_image.rows; y++) {
        int *row = (int*)label_image.ptr(y);
        for(int x=0; x < label_image.cols; x++) {
            if(row[x] != 1) {
                continue;
            }

            cv::Rect rect;
            cv::floodFill(label_image, cv::Point(x,y), label_count, &rect, 0, 0, 4);

            std::vector <cv::Point2i> blob;

            for(int i=rect.y; i < (rect.y+rect.height); i++) {
                int *row2 = (int*)label_image.ptr(i);
                for(int j=rect.x; j < (rect.x+rect.width); j++) {
                    if(row2[j] != label_count) {
                        continue;
                    }

                    blob.push_back(cv::Point2i(j,i));
                }
            }

            blobs.push_back(blob);

            label_count++;
        }
    }
}


int main(int argc, char* argv[])
{
    Ticker t;
    VideoCapture cap(0); // open the video file for reading
    if ( !cap.isOpened() )  // if not success, exit program
    {
        cout << "Cannot open the video file" << endl;
        return -1;
    }

    //cap.set(CV_CAP_PROP_POS_MSEC, 300); //start the video at 300ms
    double fps = cap.get(CV_CAP_PROP_FPS); //get the frames per seconds of the video
    cout << "Frame per seconds : " << fps << endl;
    namedWindow("MyVideo",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"


    while(1)
    {
        Mat frame;
        Mat small;
        Mat filtered;
        Mat binary;

        vector<vector<Point2i> > blobs;

        bool bSuccess = cap.read(frame); // read a new frame from video

        if (!bSuccess) //if not success, break loop
        {
            cout << "Cannot read the frame from video file" << endl;
            break;
        }

        //downsample
        pyrDown(frame,small,Size(frame.cols/2.0,frame.rows/2.0));

        //threshold
        inRange(small, Scalar(250,250,250),Scalar(255,255,255), filtered);

        cv::threshold(filtered, binary, 0.0, 1.0, cv::THRESH_BINARY);

        //find countours
        FindBlobs(binary,blobs);
        cv::Mat output = cv::Mat::zeros(binary.size(), CV_8UC3);

        for(size_t i=0; i < blobs.size(); i++) {
            unsigned char r = 255 * (rand()/(1.0 + RAND_MAX));
            unsigned char g = 255 * (rand()/(1.0 + RAND_MAX));
            unsigned char b = 255 * (rand()/(1.0 + RAND_MAX));

            for(size_t j=0; j < blobs[i].size(); j++) {
                int x = blobs[i][j].x;
                int y = blobs[i][j].y;

                output.at<cv::Vec3b>(y,x)[0] = b;
                output.at<cv::Vec3b>(y,x)[1] = g;
                output.at<cv::Vec3b>(y,x)[2] = r;
            }
        }

        t.tick();

        imshow("MyVideo", output); //show the frame in "MyVideo" window

        if(waitKey(30) == 27) //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
        {
            cout << "esc key is pressed by user" << endl; 
            break; 
        }
    }

    return 0;
}
