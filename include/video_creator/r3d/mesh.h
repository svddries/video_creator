#ifndef _MESH_H_
#define _MESH_H_

#include "video_creator/math/math.h"
#include <vector>

namespace r3d
{

struct Triangle
{
    Triangle(unsigned int i1_, unsigned int i2_, unsigned int i3_)
        : i1(i1_), i2(i2_), i3(i3_) {}

    union {
        struct { unsigned int i1, i2, i3; };
        unsigned int i[3];
    };

    Vec3 normal;
};

struct Mesh
{
    std::vector<Vec3> vertices;
    std::vector<Triangle> triangles;

    inline unsigned int addPoint(const Vec3f& p)
    {
        vertices.push_back(p);
        return vertices.size() - 1;
    }

    inline void addTriangle(unsigned int i1, unsigned int i2, unsigned int i3) { triangles.push_back(Triangle(i1, i2, i3)); }

    void calculateNormals();
};
} // end namespace r3d

#endif
