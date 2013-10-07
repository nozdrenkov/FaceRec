#if 1
#include <iostream>
#include <vector>
#include <string>
#include <clocale>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <ctime>
#include <sstream>
using namespace std;
using namespace cv;

void drawPoint(Mat &img, const Point &pt, const Scalar &color)
{
    line(img, pt - Point(5, 0), pt + Point(5, 0), color, 1);
    line(img, pt - Point(0, 5), pt + Point(0, 5), color, 1);
}

void drawRectangle(Mat &img, const Rect &tr, const Scalar &color)
{
    // putText(img, "Text", Point(tr.x, tr.y - 5), FONT_HERSHEY_COMPLEX_SMALL, 0.8, CV_RGB(0, 255, 0));
    Rect r = tr;
    rectangle(img, r, color, 1);
    Point center(r.x + r.width / 2, r.y + r.height / 2);
    drawPoint(img, center, color);
}

string toString(double x)
{
    ostringstream os;
    os.setf(ios::fixed);
    os.precision(3);
    os << x;
    return os.str();
}

void drawTime(Mat &image, double time)
{
    const string text = toString(1 / time) + " FPS";
    putText(image, text, Point(15, 15), FONT_HERSHEY_COMPLEX_SMALL, 0.8, CV_RGB(0, 255, 0));
}

int main()
{
    setlocale(LC_ALL, "rus");

    namedWindow("detector", CV_WINDOW_AUTOSIZE);
    const string path = "C:\\v1.mp4";
    VideoCapture capture(path);

    if (capture.isOpened())
    {
        while (capture.grab())
        {
            Mat source_frame;
            capture.retrieve(source_frame);
            
            
            time_t T1 = clock();

            Mat frame;
            resize(source_frame, frame, Size(), 0.3, 0.3);

            CascadeClassifier cascade;
            if (!cascade.load("C:\\opencv\\data\\haarcascades\\haarcascade_frontalface_alt.xml"))
            {
                cout << "Cascade loading error!" << endl;
                break;
            }

            vector<Rect> faces;
            cascade.detectMultiScale(frame, faces, 1.18);

            time_t T2 = clock();
            double time = double(T2 - T1) / CLOCKS_PER_SEC;


            for (int i = 0; i < faces.size(); ++i)
                drawRectangle(frame, faces[i], CV_RGB(0, 255, 0));

            drawTime(frame, time);
            

            imshow("detector", frame);

            char c = waitKey(1);
            if (c == 27)
                break;
        }
    }
    
    return 0;
}
#endif