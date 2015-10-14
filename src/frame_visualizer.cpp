#include "video_creator/frame_visualizer.h"

#include <opencv2/highgui/highgui.hpp>

#include <unistd.h> // usleep

// ----------------------------------------------------------------------------------------------------

FrameVisualizer::FrameVisualizer(int width, int height)
    : width_(width), height_(height), background_clr_(0, 0, 0)
{
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

cv::Mat& FrameVisualizer::nextFrame()
{
    if (!draw_canvas_.data)
    {
        draw_canvas_ = cv::Mat(height_, width_, CV_8UC3, background_clr_);
        show_canvas_ = cv::Mat(height_, width_, CV_8UC3, background_clr_);
        return draw_canvas_;
    }

    double dt_sec = timer_.getElapsedTimeInSec();

    double wait_time_sec = dt() - dt_sec;

    if (wait_time_sec > 0)
        usleep(wait_time_sec * 1000000);

    timer_.reset();

    // Swap canvas
    cv::Mat tmp = show_canvas_;
    show_canvas_ = draw_canvas_;
    draw_canvas_ = tmp;

    cv::imshow("video", show_canvas_);
    char key = cv::waitKey(3);
    if (key == 'q')
        exit(1);


    draw_canvas_.setTo(background_clr_);

    return draw_canvas_;
}

