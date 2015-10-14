#include <video_creator/frame_visualizer.h>

int main(int argc, char **argv)
{
    FrameVisualizer f(800, 600);
    f.setBackgroundColor(cv::Scalar(255, 255, 255));
    f.setFrameRate(60);
    cv::Mat canvas;

    int x = 0;
    int v = 800 / f.fps();
    while(true)
    {
        canvas = f.nextFrame();
        cv::circle(canvas, cv::Point(x, 100), 10, cv::Scalar(0, 0, 255));

        x += v;
        if (x > 800 || x < 0)
            v = -v;

    }

    canvas = f.nextFrame();

    return 0;
}
