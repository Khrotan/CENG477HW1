#ifndef RAYTRACER_MESH_H
#define RAYTRACER_MESH_H

#include <vector>
#include "Vertex.h"
#include "Triangle.h"

class Mesh {
public:
    int triangleCount;
    int materialId;

    std::vector<Triangle> triangles;

    bool Intersect(const Ray& ray, RayHitInfo& hitInfo) const;
};
#endif //RAYTRACER_MESH_H
