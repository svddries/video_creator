#ifndef _COLOR_H_
#define _COLOR_H_

#include <opencv2/core/core.hpp>

struct Color
{

    Color() {}

    Color(float r_, float g_, float b_) : r(r_), g(g_), b(b_) {}

    union {
        struct { float r, g, b; };
        float channel[3];
    };

    cv::Vec3b toCv() const { return cv::Vec3b(b * 255, g * 255, r * 255); }

};

#endif
