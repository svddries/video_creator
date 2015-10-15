#include "video_creator/frame_visualizer.h"

#include <opencv2/highgui/highgui.hpp>

#include <unistd.h> // usleep

// ----------------------------------------------------------------------------------------------------

FrameVisualizer::FrameVisualizer(int width, int height, bool full_screen)
    : width_(width), height_(height), background_clr_(0, 0, 0)
{
    if (full_screen)
    {
        cv::namedWindow("video", CV_WINDOW_NORMAL);
        cv::setWindowProperty("video", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    }
}

// ----------------------------------------------------------------------------------------------------

FrameVisualizer::~FrameVisualizer()
{
}

// ----------------------------------------------------------------------------------------------------

void FrameVisualizer::pause()
{
    char key = cv::waitKey();
    if (key == 'q')
        exit(1);
}

// ----------------------------------------------------------------------------------------------------

cv::Mat FrameVisualizer::createFrame() const
{
    return cv::Mat(height_, width_, CV_8UC3, background_clr_);
}

// ----------------------------------------------------------------------------------------------------

void FrameVisualizer::show(const cv::Mat& canvas)
{
    double dt_sec = timer_.getElapsedTimeInSec();

    double wait_time_sec = dt() - dt_sec;

    if (wait_time_sec > 0)
        usleep(wait_time_sec * 1000000);

    timer_.reset();

    cv::imshow("video", canvas);
    char key = cv::waitKey(3);
    if (key == 'q')
        exit(1);
}

