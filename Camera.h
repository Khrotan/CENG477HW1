#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "Vector3.h"
#include "Image.h"
#include "Ray.h"

class Camera {
public:
    struct {
        // v: up, u: left w: forward
        Vector3 Forward, Up, Left;
    } _space;

    struct {
        float Left, Right, Bottom, Top;
        float Distance;

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

    const std::string& OutputFile() const
    {
        return _outputFile;
    }
};

Image Camera::Render() const
{
    for ( int i = 0 ; i < this->_imagePlane.Width ; i++ ) {
        for ( int j = 0 ; j < this->_imagePlane.Height ; j++ ) {
            double s_u = ( _imagePlane.Right - _imagePlane.Left ) * ( ((double) i ) + 0.5 ) / (_imagePlane.Width);
            double s_v = ( _imagePlane.Right - _imagePlane.Left ) * ( ((double) j ) + 0.5 ) / (_imagePlane.Height);

            Vector3 m = this->_position + this->_gaze * this->_imagePlane.Distance;
            Vector3 q = m + this->_space.Left * this->_imagePlane.Left + this->_space.Up * this->_imagePlane.Top;
            Vector3 s = q + this->_space.Left * s_u - this->_space.Up * s_v;

            Vector3 d = s - this->_position;

        }
    }

    return Image(0, 0);
}

#endif //RAYTRACER_CAMERA_H
