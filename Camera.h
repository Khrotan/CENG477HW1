#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "Vector3.h"
#include "Image.h"
#include "Ray.h"
#include "Cube.h"
#include "PointLight.h"
#include "Rotation.h"
#include "Sphere.h"

class Camera {
public:
    struct {
        // v: up, u: left w: forward
        Vector3 Forward, Up, Left;
    } _space;

    struct {
        double Left, Right, Bottom, Top;
        double Distance;

        int Width;
        int Height;

    } _imagePlane;

    Vector3 _position;
    Vector3 _gaze;

    std::string _outputFile;

    /*
     * BEWARE! You should render each camera by using this function, we will grade it according to regulations!
     */
    Image Render() const;

    Color shade( PointLight light, RayHitInfo closestRayHitInfo, Ray ray, int recursionDepth ) const;

    bool fillHitInfo( RayHitInfo& closestRayHitInfo, Ray ray ) const;

    void findI_J_cube( RayHitInfo& info ) const;

    void findI_J( RayHitInfo& info, Sphere sphere ) const;

    const std::string& OutputFile() const
    {
        return _outputFile;
    }
};

#endif //RAYTRACER_CAMERA_H
