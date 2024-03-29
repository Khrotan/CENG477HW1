#ifndef RAYTRACER_TRIANGLE_H
#define RAYTRACER_TRIANGLE_H

#include "Ray.h"

class Triangle
{
public:
    Vector3 Vid1;
    Vector3 Vid2;
    Vector3 Vid3;

    Vector3 normal;

    Triangle( const Vector3& Vid1, const Vector3& Vid2, const Vector3& Vid3 );

    void computeNormal();

    // This function gets a ray, and if the triangle intersects with the ray, returns true and fills the hitInfo struct
    // Else return false and leave hitInfo unchanged
    bool Intersect(const Ray& ray, RayHitInfo& hitInfo) const;
};

#endif //RAYTRACER_TRIANGLE_H

