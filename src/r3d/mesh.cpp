#include "video_creator/r3d/mesh.h"

namespace r3d
{

void Mesh::calculateNormals()
{
    for(unsigned int i = 0; i < triangles.size(); ++i)
    {
        Triangle& t = triangles[i];

        const Vec3& p1 = vertices[t.i1];
        const Vec3& p2 = vertices[t.i2];
        const Vec3& p3 = vertices[t.i3];

        t.normal = ((p3 - p1).cross(p2 - p1)).normalized();
    }
}

} // end namespace r3d

