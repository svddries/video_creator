#include <video_creator/frame_visualizer.h>

#include <video_creator/r3d/primitives.h>
#include <video_creator/r3d/mesh.h>
#include <video_creator/r3d/camera.h>

#include <video_creator/text/text.h>

int main(int argc, char **argv)
{
    FrameVisualizer f(1280, 800, false);
    f.setBackgroundColor(cv::Scalar(255, 255, 255));
    f.setFrameRate(50);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // Set Title text
    cv::Mat title_screen = f.createFrame();

    text::Font font;
    font.color = Color(0, 0, 0);
    font.name = "Garuda";
    font.size = 0.05;

    text::addText(title_screen, "World Modeling in Robotics", font, 0.5, 0.1);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    r3d::Mesh box;
    r3d::createBox(Vec3f(-0.5, -0.5, -0.5), Vec3f(0.5, 0.5, 0.5), box);

    r3d::Camera camera;
    camera.setFocalLengths(0.87, 0.87);
    camera.setOpticalCenter(0.5, 0.5);
    camera.setOpticalTranslation(0, 0);

    Transform3f pose;
    pose.t = Vec3f(0, 0, -3);
    pose.R.setRPY(0.5, 0.5, 0.5);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    double a = 0;
    while(true)
    {
        cv::Mat canvas = title_screen.clone();
        cv::Mat roi = canvas(cv::Rect(cv::Point(f.width() - 100, f.height() - 100), cv::Size(100, 100)));
        r3d::Canvas3D canvas3d(roi);

        pose.R.setRPY(0.5, a, 0.5);
        camera.rasterize(box, cv::Vec3b(255, 100, 0), pose, canvas3d);

        f.show(canvas);

        a += 2.4 * f.dt();
    }

    f.pause();

    return 0;
}
