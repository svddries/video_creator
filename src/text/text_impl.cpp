#include "video_creator/text/text_impl.h"

using namespace std;

namespace text
{

Text::Text(double x, double y, const std::string& font, int size, double red, double green, double blue, const std::string& text, bool bold, double edge_thickness, Alignment alignment)
    : x_(x), y_(y), font_(font), size_(size), red_(red), green_(green), blue_(blue), text_(text), bold_(bold), edge_thickness_(edge_thickness),
      alignment_(alignment), scale_x_(1), surface_(0), cr_(0)
{
}

Text::~Text() {

}

void Text::setAlignment(Alignment alignment) {
    alignment_ = alignment;
}

bool Text::initialize(cv::Mat& image) {
    surface_ = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, image.cols, image.rows);
    cr_ = cairo_create(surface_);

    if (bold_) {
        cairo_select_font_face(cr_, font_.c_str(), CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    } else {
        cairo_select_font_face(cr_, font_.c_str(), CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    }

    cairo_set_font_size (cr_, size_);

    cairo_text_extents_t extents;
    cairo_text_extents (cr_, text_.c_str(), &extents);

    if (alignment_ == CENTER) {
        cairo_move_to(cr_, x_ - (extents.width/2 + extents.x_bearing), y_ - (extents.height/2 + extents.y_bearing));

        x_min_ = max(0, x_ - (int)(extents.width/2 + extents.x_bearing) - 10);
        x_max_ = min(image.cols, x_ + (int)(extents.width/2 + extents.x_bearing) + 10);

    } else {
        cairo_move_to(cr_, x_, y_ - (extents.height/2 + extents.y_bearing));

        x_min_ = max(0, x_);
        x_max_ = min(image.cols, x_ + (2 * (int)(extents.width + extents.x_bearing) + 10));

    }

    y_min_ = max(0, y_ - (int)extents.height/2 - 10);
    y_max_ = min(image.rows, y_ + (int)extents.height/2 + 10);

    cairo_set_source_rgb (cr_, red_, green_, blue_);

    if (edge_thickness_ == 0) {
        // no outline
        cairo_show_text (cr_, text_.c_str());
    } else {
        // outline
        cairo_text_path (cr_, text_.c_str());
        cairo_fill_preserve (cr_);
        cairo_set_source_rgb (cr_, 0, 0, 0);
        cairo_set_line_width (cr_, edge_thickness_);
        cairo_stroke (cr_);
    }

    return true;
}

void Text::setWidthFactor(double width_factor) {
    scale_x_ = width_factor;
}

bool Text::apply(cv::Mat& image) {

    unsigned char fade_alpha = 255;

    unsigned char* cairo_data = cairo_image_surface_get_data(surface_);

    double x_center = (x_max_ + x_min_) / 2;

    for(int y = y_min_; y < y_max_; y++) {
        for(int x = x_min_; x < x_max_; x++) {

            int i_cairo = (x + y * image.cols) * 4;

            double rx = x_center + (x - x_center) * scale_x_;

            cv::Vec3b& current_color = image.at<cv::Vec3b>(y, rx);

            unsigned char alpha = cairo_data[i_cairo+3];
            if (fade_alpha < 255) {
                alpha = alpha * fade_alpha / 255;
            }

            int pr = (cairo_data[i_cairo + 0] * alpha + current_color[0] * (255 - alpha)) / 255;
            int pg = (cairo_data[i_cairo + 1] * alpha + current_color[1] * (255 - alpha)) / 255;
            int pb = (cairo_data[i_cairo + 2] * alpha + current_color[2] * (255 - alpha)) / 255;

            image.at<cv::Vec3b>(y, rx) = cv::Vec3b(pr, pg, pb);
        }
    }

    return true;
}

}
