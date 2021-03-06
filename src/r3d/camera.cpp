#include "video_creator/r3d/camera.h"
#include "video_creator/r3d/mesh.h"

namespace r3d
{

// ----------------------------------------------------------------------------------------------------

void sort(const Vec3f& p1, const Vec3f& p2, const Vec3f& p3, int i,
                       Vec3f& p_min, Vec3f& p_mid, Vec3f& p_max)
{

    if (p1.m[i] < p2.m[i]) {
        if (p2.m[i] < p3.m[i]) {
            p_min = p1; p_mid = p2; p_max = p3;
        } else if (p3.m[i] < p1.m[i]) {
            p_min = p3; p_mid = p1; p_max = p2;
        } else {
            p_min = p1; p_mid = p3; p_max = p2;
        }
    } else {
        if (p1.m[i] < p3.m[i]) {
            p_min = p2; p_mid = p1; p_max = p3;
        } else if (p3.m[i] < p2.m[i]) {
            p_min = p3; p_mid = p2; p_max = p1;
        } else {
            p_min = p2; p_mid = p3; p_max = p1;
        }
    }
}

// ----------------------------------------------------------------------------------------------------

void Camera::rasterize(const Mesh& mesh, const cv::Vec3b& color, const Transform3f& pose, Canvas3D& canvas) const
{

//    if (mesh.getMaxRadius() < pose.getOrigin().z) {
//        return;
//    }

    const std::vector<Vec3f>& vertices = mesh.vertices;
    const std::vector<Triangle>& triangles = mesh.triangles;

    // transform points
    std::vector<Vec3f> vertices_t(vertices.size());
    std::vector<Vec2f> vertices_proj(vertices.size());

    for(unsigned int i = 0; i < vertices.size(); ++i)
    {
        vertices_t[i] = pose * vertices[i];
        vertices_proj[i] = project3Dto2D(vertices_t[i], canvas.depth.cols);
    }

    for(std::vector<Triangle>::const_iterator it_tri = triangles.begin(); it_tri != triangles.end(); ++it_tri)
    {
        const Triangle& t = *it_tri;

        const Vec3f& p1_3d = vertices_t[t.i1];
        const Vec3f& p2_3d = vertices_t[t.i2];
        const Vec3f& p3_3d = vertices_t[t.i3];

        int n_verts_in = 0;
        bool v1_in = false;
        bool v2_in = false;
        bool v3_in = false;
        const Vec3f* vIn[3];

        if (p1_3d.z < near_clip_z_)
        {
            ++n_verts_in;
            v1_in = true;
        }

        if (p2_3d.z < near_clip_z_)
        {
            ++n_verts_in;
            v2_in = true;
        }

        if (p3_3d.z < near_clip_z_)
        {
            ++n_verts_in;
            v3_in = true;
        }

        if (n_verts_in == 1)
        {
            if (v1_in) { vIn[0] = &(p1_3d); vIn[1] = &(p2_3d); vIn[2] = &(p3_3d); }
            if (v2_in) { vIn[0] = &(p2_3d); vIn[1] = &(p3_3d); vIn[2] = &(p1_3d); }
            if (v3_in) { vIn[0] = &(p3_3d); vIn[1] = &(p1_3d); vIn[2] = &(p2_3d); }

            //Parametric line stuff
            Vec3f v01 = *vIn[1] - *vIn[0];

            float t1 = ((near_clip_z_ - (*vIn[0]).z) / v01.z );

            Vec3f new2(vIn[0]->x + v01.x * t1, vIn[0]->y + v01.y * t1, near_clip_z_);

            // Second vert point
            Vec3f v02 = *vIn[2] - *vIn[0];

            float t2 = ((near_clip_z_ - (*vIn[0]).z) / v02.z);

            Vec3f new3(vIn[0]->x + v02.x * t2, vIn[0]->y + v02.y * t2, near_clip_z_);

            drawTriangle(*vIn[0], new2, new3, t, pose, color, canvas);
        }
        else if (n_verts_in == 2)
        {
            if (!v1_in) { vIn[0]=&(p2_3d); vIn[1]=&(p3_3d); vIn[2]=&(p1_3d); }
            if (!v2_in) { vIn[0]=&(p3_3d); vIn[1]=&(p1_3d); vIn[2]=&(p2_3d); }
            if (!v3_in) { vIn[0]=&(p1_3d); vIn[1]=&(p2_3d); vIn[2]=&(p3_3d); }

            //Parametric line stuff
            Vec3f v01 = *vIn[2] - *vIn[0];

            float t1 = ((near_clip_z_ - (*vIn[0]).z)/v01.z );

            Vec3f new2((*vIn[0]).x + v01.x * t1,(*vIn[0]).y + v01.y * t1, near_clip_z_);

            // Second point
            Vec3f v02 = *vIn[2] - *vIn[1];

            float t2 = ((near_clip_z_ - (*vIn[1]).z)/v02.z);

            Vec3f new3((*vIn[1]).x + v02.x * t2, (*vIn[1]).y + v02.y * t2, near_clip_z_);

            drawTriangle(*vIn[0], *vIn[1], new2, t, pose, color, canvas);

            drawTriangle(new2, *vIn[1], new3, t, pose, color,canvas);

        }
        else if (n_verts_in == 3)
        {
            const Vec2f& p1_2d = vertices_proj[it_tri->i1];
            const Vec2f& p2_2d = vertices_proj[it_tri->i2];
            const Vec2f& p3_2d = vertices_proj[it_tri->i3];

            drawTriangle2D(Vec3f(p1_2d.x, p1_2d.y, 1.0f / -p1_3d.z),
                           Vec3f(p2_2d.x, p2_2d.y, 1.0f / -p2_3d.z),
                           Vec3f(p3_2d.x, p3_2d.y, 1.0f / -p3_3d.z),
                           t, pose, color, canvas);
        }
    }
}

// -------------------------------------------------------------------------------

void Camera::drawTriangle(const Vec3f& p1_3d, const Vec3f& p2_3d, const Vec3f& p3_3d, const Triangle& t,
                          const Transform3f& pose, const cv::Vec3b& color, Canvas3D& canvas) const
{
    Vec2f p1_2d = project3Dto2D(p1_3d, canvas.depth.cols);
    Vec2f p2_2d = project3Dto2D(p2_3d, canvas.depth.cols);
    Vec2f p3_2d = project3Dto2D(p3_3d, canvas.depth.cols);

    drawTriangle2D(Vec3f(p1_2d.x, p1_2d.y, 1.0f / -p1_3d.z),
                   Vec3f(p2_2d.x, p2_2d.y, 1.0f / -p2_3d.z),
                   Vec3f(p3_2d.x, p3_2d.y, 1.0f / -p3_3d.z),
                   t, pose, color, canvas);
}

// -------------------------------------------------------------------------------

void Camera::drawTriangle2D(const Vec3f& p1, const Vec3f& p2, const Vec3f& p3, const Triangle& t,
                            const Transform3f& pose, const cv::Vec3b& color, Canvas3D& canvas) const
{

    if (!back_face_culling_ || (p2.x - p1.x) * (p3.y - p1.y) - (p3.x - p1.x) * (p2.y - p1.y) < 0)
    {
        int min_y = std::min<int>(p1.y, std::min<int>(p2.y, p3.y));
        int max_y = std::max<int>(p1.y, std::max<int>(p2.y, p3.y));
        int min_x = std::min<int>(p1.x, std::min<int>(p2.x, p3.x));
        int max_x = std::max<int>(p1.x, std::max<int>(p2.x, p3.x));

        if (min_x < canvas.depth.cols || max_x > 0 || min_y < canvas.depth.rows || max_y > 0)
        {
            // calculate normal and color here
            Vec3f normal = pose.R * t.normal;
            float shade = (1 + normal.dot(Vec3(0, 0.3, -1).normalized())) / 2;
            cv::Vec3b clr = shade * color;

            if (min_y == max_y) {
                Vec3f p_min, p_mid, p_max;
                sort(p1, p2, p3, 0, p_min, p_mid, p_max);

                drawTrianglePart(p_min.y, p_mid.y,
                     p_min.x, 0, p_max.x, 0,
                     p_min.z, 0, p_max.z, 0,
                     clr, canvas);
            } else {
                Vec3f p_min, p_mid, p_max;
                sort(p1, p2, p3, 1, p_min, p_mid, p_max);

                int y_min_mid = (int)p_mid.y - (int)p_min.y;
                int y_mid_max = (int)p_max.y - (int)p_mid.y;
                int y_min_max = (int)p_max.y - (int)p_min.y;

                Vec3f p_prime = (y_mid_max * p_min + y_min_mid * p_max) / y_min_max;

                Vec3f p_a, p_b;
                if (p_prime.x < p_mid.x) {
                    p_a = p_prime; p_b = p_mid;
                } else {
                    p_a = p_mid; p_b = p_prime;
                }

                drawTrianglePart(p_min.y, p_mid.y,
                     p_min.x, (p_a.x - p_min.x) / y_min_mid, p_min.x, (p_b.x - p_min.x) / y_min_mid,
                     p_min.z, (p_a.z - p_min.z) / y_min_mid, p_min.z, (p_b.z - p_min.z) / y_min_mid,
                     clr, canvas);

                drawTrianglePart(p_mid.y, p_max.y,
                     p_a.x, (p_max.x - p_a.x) / y_mid_max, p_b.x, (p_max.x - p_b.x) / y_mid_max,
                     p_a.z, (p_max.z - p_a.z) / y_mid_max, p_b.z, (p_max.z - p_b.z) / y_mid_max,
                     clr, canvas);

            }
        }
    }
}

// -------------------------------------------------------------------------------

void Camera::drawTrianglePart(int y_start, int y_end,
                                  float x_start, float x_start_delta, float x_end, float x_end_delta,
                                  float d_start, float d_start_delta, float d_end, float d_end_delta,
                                  const cv::Vec3b& clr, Canvas3D& canvas) const
{
    if (y_start < 0)
    {
        d_start += d_start_delta * -y_start;
        d_end += d_end_delta * -y_start;
        x_start += x_start_delta * -y_start;
        x_end += x_end_delta * -y_start;
        y_start = 0;
    }

    y_end = std::min<int>(canvas.depth.rows - 1, y_end);

    for(int y = y_start; y <= y_end; ++y)
    {
        float d = d_start;
        float d_delta = (d_end - d_start) / (x_end - x_start);

        int x_start2;
        if (x_start < 0)
        {
            d += d_delta * -x_start;
            x_start2 = 0;
        }
        else
        {
            x_start2 = x_start;
        }

        int x_end2 = std::min<int>(canvas.depth.cols - 1, (int)x_end);

        for(int x = x_start2; x <= x_end2; ++x)
        {
            float depth = 1.0f / d;

            int i_pixel = y * canvas.depth.cols + x;

            float& old_depth = canvas.depth.at<float>(i_pixel);

            // Render pixel
            if (old_depth == 0 || old_depth > depth)
            {
                old_depth = depth;
                canvas.rgb.at<cv::Vec3b>(i_pixel) = clr;
            }

            d += d_delta;
        }

        d_start += d_start_delta;
        d_end += d_end_delta;
        x_start += x_start_delta;
        x_end += x_end_delta;
    }
}

} // end namespace r3d

