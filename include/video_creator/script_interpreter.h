#ifndef VIDEO_CREATOR_SCRIPT_INTERPRETER_H_
#define VIDEO_CREATOR_SCRIPT_INTERPRETER_H_

#include <string>
#include <opencv2/core/core.hpp>

#include "video_creator/text/text.h"

class FrameVisualizer;

class ScriptInterpreter
{

public:

    ScriptInterpreter(FrameVisualizer* f);

    ~ScriptInterpreter();

    bool open(const std::string& filename);

private:

    void drawText(const std::string& text, cv::Mat& canvas);

    std::string executeSpecial(const std::string& special, cv::Mat& canvas);

    FrameVisualizer* f_;

    double x_;
    double y_;

    text::Font font_;

};

#endif
