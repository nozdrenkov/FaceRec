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

int main()
{
    const string path = "C:\\v1.mp4";

    VideoCapture cap(path);

    Mat frame;
    while (cap.grab())
    {
        cap.retrieve(frame);
        resize(frame, frame, Size(), 0.5, 0.5);

        imshow("detector", frame);

        char c = waitKey(50);
        if (c == 27)
            break;
    }
}

#endif