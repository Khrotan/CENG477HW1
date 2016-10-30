//
// Created by Arda on 28.10.2016.
//

#include "Triangle.h"

bool Triangle::Intersect( const Ray& ray, RayHitInfo& hitInfo ) const
{
    double a_x_b_x = this->Vid1._data[0] - this->Vid2._data[0];
    double a_y_b_y = this->Vid1._data[1] - this->Vid2._data[1];
    double a_z_b_z = this->Vid1._data[2] - this->Vid2._data[2];

    double a_x_c_x = this->Vid1._data[0] - this->Vid3._data[0];
    double a_y_c_y = this->Vid1._data[1] - this->Vid3._data[1];
    double a_z_c_z = this->Vid1._data[2] - this->Vid3._data[2];

    double d_x = ray._direction._data[0];
    double d_y = ray._direction._data[1];
    double d_z = ray._direction._data[2];

    double a_x_o_x = this->Vid1._data[0] - ray._origin._data[0];
    double a_y_o_y = this->Vid1._data[1] - ray._origin._data[1];
    double a_z_o_z = this->Vid1._data[2] - ray._origin._data[2];

    double det_A = a_x_b_x*( a_y_c_y*d_z - d_y*a_z_c_z ) + a_y_b_y*( d_x*a_z_c_z - a_x_c_x*d_z ) + a_z_b_z*( a_x_c_x*d_y - a_y_c_y*d_x );

    double det_beta = a_x_o_x*( a_y_c_y*d_z - a_z_c_z*d_y ) + a_y_o_y*( a_z_c_z*d_x - a_x_c_x*d_z ) + a_z_o_z*( a_x_c_x*d_y - a_y_c_y*d_x );
    double det_alpha = a_x_b_x*( a_y_o_y*d_z - a_z_o_z*d_y ) + a_y_b_y*( a_z_o_z*d_x - a_x_o_x*d_z ) + a_z_b_z*( a_x_o_x*d_y - a_y_o_y*d_x );
    double det_t = a_x_b_x*( a_y_c_y*a_z_o_z - a_z_c_z*a_y_o_y ) + a_y_b_y*( a_z_c_z*a_x_o_x - a_x_c_x*a_z_o_z ) + a_z_b_z*( a_x_c_x*a_y_o_y - a_y_c_y*a_x_o_x );

    double beta = det_beta/det_A;
    double alpha = det_alpha/det_A;
    double t = det_t/det_A;

    if ( ( (beta + alpha) <= 1 ) && 0 <= beta && 0 <= alpha ) {
        hitInfo.Parameter = t;
        hitInfo.Position = ray._origin + ray._direction * t;
        hitInfo.Normal = this->normal;

        return true;
    }

    return false;
}