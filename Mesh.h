#ifndef RAYTRACER_MESH_H
#define RAYTRACER_MESH_H

#include "Vertex.h"

class Mesh {
public:
    int triangleCount;
    int materialId;
    Vector3 pos1;
    Vector3 pos2;
    Vector3 pos3;

    bool Intersect(const Ray& ray, RayHitInfo& hitInfo) const;
};

#endif //RAYTRACER_MESH_H
