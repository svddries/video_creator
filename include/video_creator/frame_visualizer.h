#ifndef VIDEO_CREATOR_FRAME_VISUALIZER_H_
#define VIDEO_CREATOR_FRAME_VISUALIZER_H_

#include "frame_processor.h"
#include "timer.h"

#include <opencv2/core/core.hpp>

class FrameVisualizer : public FrameProcessor
{

public:

    FrameVisualizer(int width, int height);

    ~FrameVisualizer();

    cv::Mat& nextFrame();

    void setBackgroundColor(const cv::Scalar& clr) { background_clr_ = clr; }

    void setFrameRate(double fps) { fps_ = fps; }

    double dt() const { return 1.0 / fps_; }

    double fps() const { return fps_; }

    void pause();

    int width() const { return width_; }

    int height() const { return height_; }

private:

    int width_;
    int height_;

    cv::Scalar background_clr_;

    cv::Mat show_canvas_;
    cv::Mat draw_canvas_;

    double fps_;

    Timer timer_;

};

#endif
