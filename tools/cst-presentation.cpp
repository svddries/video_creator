#include <video_creator/frame_visualizer.h>

#include <video_creator/r3d/primitives.h>
#include <video_creator/r3d/mesh.h>
#include <video_creator/r3d/camera.h>

int main(int argc, char **argv)
{
    FrameVisualizer f(800, 600);
    f.setBackgroundColor(cv::Scalar(255, 255, 255));
    f.setFrameRate(50);
    cv::Mat canvas;

    r3d::Mesh box;
    r3d::createBox(Vec3f(-0.5, -0.5, -0.5), Vec3f(0.5, 0.5, 0.5), box);

    r3d::Camera camera;
    camera.setFocalLengths(0.87 * f.width(), 0.87 * f.width());
    camera.setOpticalCenter(f.width() / 2 + 0.5, f.height() / 2 + 0.5);
    camera.setOpticalTranslation(0, 0);
    camera.setCanvasSize(f.width(), f.height());

    Transform3f pose;
    pose.t = Vec3f(0, 0, -5);
    pose.R.setRPY(0.5, 0.5, 0.5);

    double a = 0;
    while(true)
    {
        pose.R.setRPY(0.5, a, 0.5);
        canvas = f.nextFrame();
        r3d::Canvas3D canvas3d(canvas);
        camera.rasterize(box, cv::Vec3b(255, 100, 0), pose, canvas3d);
        a += 2.4 * f.dt();
    }

    f.pause();

    return 0;
}
