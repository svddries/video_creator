#ifndef VIDEO_CREATOR_TEXT_H_
#define VIDEO_CREATOR_TEXT_H_

#include <string>
#include <opencv2/core/core.hpp>

#include "video_creator/color.h"

namespace text
{

struct Font
{
    Font() : bold(false), color(0, 0, 0), size(10) {}

    std::string name;
    bool bold;
    Color color;
    double size;
};

void addText(cv::Mat& img, const std::string& text, const Font& font, double x, double y);

} // end namespace text

#endif
