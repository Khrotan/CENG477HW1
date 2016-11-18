#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include <Math.h>
#include <Vector>
#include "Ray.h"
#include "Translation.h"
#include "Rotation.h"
#include "Scaling.h"

class Sphere
{
public:
    std::vector<Translation> translations;
    std::vector<Rotation> rotations;
    std::vector<Scaling> scalings;
    int materialId;
    int textureId;
    double radius;
    Vector3 center;

    Sphere();

    void applyScaling( Scaling scaling );

    void applyTranlation( Translation translation );

    // This function gets a ray, and if the sphere intersects with the ray, returns true and fills the hitInfo struct
    // Else return false and leave hitInfo unchanged
    bool Intersect( const Ray& ray, RayHitInfo& hitInfo ) const;
};


#endif //RAYTRACER_SPHERE_H
