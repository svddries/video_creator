#ifndef _Text_H_
#define _Text_H_

#include "cv.h"

#include "cairo/cairo-ft.h"
#include "cairo/cairo.h"

namespace text
{

class Text {

public:

    enum Alignment {
        LEFT,
        CENTER,
        RIGHT
    };

    Text(double x, double y, const std::string& font, int size, double red, double green, double blue, const std::string& text, bool bold = false, double edge_thickness = 0, Alignment alignment = CENTER);

    virtual ~Text();

    bool initialize(cv::Mat& image);

    bool apply(cv::Mat& image);

    void setAlignment(Alignment alignment);

    void setWidthFactor(double width_factor);

protected:

    int x_, y_;

    int x_min_, x_max_, y_min_, y_max_;

    std::string font_;

    int size_;

    double red_, green_, blue_;

    std::string text_;

    bool bold_;

    double edge_thickness_;

    Alignment alignment_;

    double scale_x_;

    // Cairo
    cairo_surface_t* surface_;
    cairo_t* cr_;
};

}

#endif
