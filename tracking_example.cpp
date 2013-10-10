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

float dist(const Point2f &a, const Point2f &b)
{
    return sqrt((a - b).dot(a - b));
}

void drawPt(Mat &img, const Point2f &pt, const Scalar &color)
{
    Point2f dx(5, 0);
    Point2f dy(0, 5);
    line(img, pt - dx, pt + dx, color);
    line(img, pt - dy, pt + dy, color);
}

void drawVec(Mat &img, const Point2f &p1, const Point2f &p2, const Scalar &color)
{
    line(img, p1, p2, color);
    line(img, p2, p2, color, 3);
}

int circle_radius = 0;


Mat mask, selection_image;
vector<Point2f> selected_corners;

void onMouse(int event, int x, int y, int flags, void *param)
{
    switch(event)
    {
        case CV_EVENT_MOUSEMOVE:
        {
            if ( flags == 1 )
                cv::circle(mask, Point(x,y), circle_radius, Scalar(1), -1);
            break;
        }
        case CV_EVENT_LBUTTONUP:
        {
            goodFeaturesToTrack(selection_image, selected_corners, 50, 0.01, 0.1, mask, 3, false);
            break;
        }
    }
}

vector<Point2f> getCorners(const Mat &img, const Mat &gray_img)
{
    cout << "Getting corners..." << endl;
    mask.create(img.size(), CV_8UC1);
    mask.setTo(0);

    selection_image = gray_img;
    selected_corners.clear();

    setMouseCallback("detector", onMouse, nullptr);

    while (true)
    {
        Mat masked_image = img * 0.5;
        img.copyTo(masked_image, mask);


        for (size_t i = 0; i < selected_corners.size(); ++i)
            drawPt(masked_image, selected_corners[i], CV_RGB(0, 255, 0));


        imshow("detector", masked_image);

        char c = waitKey(1);
        if (c == ' ')
            break;
        if (c == 27)
            exit(0);
    }

    setMouseCallback("detector", nullptr, nullptr);

    cout << "Corners is ready!" << endl;
    return selected_corners;
}

int main()
{
    const string path = "C:\\v2.avi";

    VideoCapture cap(path);

    namedWindow("detector");

    createTrackbar("Radius", "detector", &circle_radius, 200);

    Mat prev_frame;
    vector<Point2f> prev_corners;

    Mat frame;
    while (cap.grab())
    {
        cap.retrieve(frame);
        resize(frame, frame, Size(), 0.8, 0.8);


        Mat cur_frame;
        cvtColor(frame, cur_frame, CV_BGR2GRAY);

        if (!prev_frame.empty())
        {
            if (prev_corners.empty())
                prev_corners = getCorners(frame, cur_frame);


            vector<unsigned char> status;
            vector<float> err;
            vector<Point2f> cur_corners;
            if (prev_corners.size())
                calcOpticalFlowPyrLK(prev_frame, cur_frame, prev_corners, cur_corners, status, err);


            for (size_t i = 0; i < cur_corners.size(); ++i)
            {
                if (status[i] && dist(prev_corners[i], cur_corners[i]) < 60)
                    drawVec(frame, prev_corners[i], cur_corners[i], CV_RGB(0, 255, 0));
                else
                    drawPt(frame, cur_corners[i], CV_RGB(255, 0, 0));
            }


            prev_corners = cur_corners;
            imshow("detector", frame);
        }
        prev_frame = cur_frame.clone();



        char c = waitKey(100);
        if (c == 27)
            break;
        if (c == ' ')
            prev_corners.clear();


        for (int i = 0; i < 2; ++i)
            cap.grab();
    }
}

#endif