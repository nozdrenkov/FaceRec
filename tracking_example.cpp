#if 1

#include <iostream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/video/tracking.hpp>

using namespace cv;
using namespace std;

void drawPt(Mat &img, const Point2f &pt, const Scalar &color)
{
    Point2f dx(5, 0);
    Point2f dy(0, 5);
    line(img, pt - dx, pt + dx, color);
    line(img, pt - dy, pt + dy, color);
}

int main()
{
    const string path = "C:\\v1.mp4";

    VideoCapture cap(path);

    Mat frame;
    while (cap.grab())
    {
        cap.retrieve(frame);
        resize(frame, frame, Size(), 0.5, 0.5);

        
        vector<Point2f> corners;
        Mat cur_frame;
        cvtColor(frame, cur_frame, CV_BGR2GRAY);
        goodFeaturesToTrack(cur_frame, corners, 500, 0.01, 0.1, noArray(), 3, false);

        for (size_t i = 0; i < corners.size(); ++i)
            drawPt(frame, corners[i], CV_RGB(0, 255, 0));



        imshow("detector", frame);

        char c = waitKey(50);
        if (c == 27)
            break;
    }
}

#endif