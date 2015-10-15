#include "video_creator/script_interpreter.h"
#include "video_creator/frame_visualizer.h"
#include "video_creator/text/text.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>

// ----------------------------------------------------------------------------------------------------

ScriptInterpreter::ScriptInterpreter(FrameVisualizer* f) : f_(f)
{
}

// ----------------------------------------------------------------------------------------------------

ScriptInterpreter::~ScriptInterpreter()
{
}

// ----------------------------------------------------------------------------------------------------

bool ScriptInterpreter::open(const std::string& filename)
{
    std::ifstream input(filename.c_str());

    if (!input.is_open())
    {
        std::cout << "Could not open file: " << filename << std::endl;
        return false;
    }

    font_.color = Color(0, 0, 0);
    font_.name = "Garuda";
    font_.size = 0.05;

    cv::Mat canvas;

    for(int line_nr = 1; !input.eof(); ++line_nr)
    {
        std::string line;
        std::getline(input, line);
        if (line.empty() || line[0] == '#')
            continue;

        std::size_t i = 0;
        std::size_t j = 0;
        while(true)
        {
            i = line.find('<', j);

            if (i > j)
            {
                std::string txt = line.substr(j, i - j);
                if (!txt.empty())
                    drawText(txt, canvas);
            }

            if (i == std::string::npos)
                break;

            j = line.find('>', i);
            if (j == std::string::npos)
            {
                std::cout << "Error at line " << line_nr << ": expected '>', not found." << std::endl;
                return false;
            }

            ++j;

            std::string special = line.substr(i + 1, j - i - 2);
            if (!special.empty())
            {
                std::string error_msg = executeSpecial(special, canvas);
                if (!error_msg.empty())
                {
                    std::cout << "Error at line " << line_nr << ": " << error_msg << std::endl;
                    return false;
                }
            }
        }
//        std::cout << line << std::endl;
    }
}

// ----------------------------------------------------------------------------------------------------

void ScriptInterpreter::drawText(const std::string& text, cv::Mat& canvas)
{
    text::addText(canvas, text, font_, x_, y_);
    y_ += font_.size;
}

// ----------------------------------------------------------------------------------------------------

std::string ScriptInterpreter::executeSpecial(const std::string& special, cv::Mat& canvas)
{
    std::stringstream ss(special);

    // Get command
    std::string command;
    std::getline(ss, command, ' ');

    // Get arguments
    std::vector<std::string> args;
    std::string arg;
    while (std::getline(ss, arg, ' '))
            args.push_back(arg);

    // Execute
    if (command == "frame")
    {
        canvas = f_->createFrame();
        x_ = 0.5;
        y_ = 0;
    }
    else if (command == "v")
    {
        y_ += atof(args[0].c_str());
    }
    else if (command == "font-size")
    {
        font_.size = atof(args[0].c_str());
    }
    else if (command == "font-name")
    {
        font_.name = args[0];
    }
    else if (command == "font-color")
    {
        font_.color = Color(atof(args[0].c_str()), atof(args[1].c_str()), atof(args[2].c_str()));
    }
    else if (command == "pause")
    {
        if (canvas.data)
        {
            f_->show(canvas);
            f_->pause();
        }
    }
    else
    {
        return "Unknown command: '" + command + "'.";
    }

    return "";
}
