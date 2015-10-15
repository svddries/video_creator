#include "video_creator/text/text.h"
#include "video_creator/text/text_impl.h"

namespace text
{

// ----------------------------------------------------------------------------------------------------

void addText(cv::Mat& img, const std::string& text, const Font& font, double xp, double yp)
{
    int size = img.cols * font.size;
    text::Text t(xp * img.cols, yp * img.cols, font.name, size, font.color.r, font.color.g, font.color.b, text);
    t.initialize(img);
    t.apply(img);
}

} // end namespace text
