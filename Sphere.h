#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include <Math.h>
#include "Ray.h"

class Sphere
{

public:
    int materialId;
    int textureId;
    double radius = 1;
    Vector3 center;

    // This function gets a ray, and if the sphere intersects with the ray, returns true and fills the hitInfo struct
    // Else return false and leave hitInfo unchanged
    bool Intersect( const Ray& ray, RayHitInfo& hitInfo ) const;
};


#endif //RAYTRACER_SPHERE_H
