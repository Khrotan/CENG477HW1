//
// Created by Arda on 28.10.2016.
//

#include "Triangle.h"

bool Triangle::Intersect( const Ray& ray, RayHitInfo& hitInfo ) const
{
    float a_x_b_x = this->Vid1._data[0] - this->Vid2._data[0];
    float a_y_b_y = this->Vid1._data[1] - this->Vid2._data[1];
    float a_z_b_z = this->Vid1._data[2] - this->Vid2._data[2];

    float a_x_c_x = this->Vid1._data[0] - this->Vid3._data[0];
    float a_y_c_y = this->Vid1._data[1] - this->Vid3._data[1];
    float a_z_c_z = this->Vid1._data[2] - this->Vid3._data[2];

    float d_x = ray._direction._data[0];
    float d_y = ray._direction._data[1];
    float d_z = ray._direction._data[2];

    float a_x_o_x = this->Vid1._data[0] - ray._origin._data[0];
    float a_y_o_y = this->Vid1._data[1] - ray._origin._data[1];
    float a_z_o_z = this->Vid1._data[2] - ray._origin._data[2];

    float det_A = a_x_b_x * ( a_y_c_y * d_z - a_z_c_z * d_y ) + a_y_b_y * ( d_x * a_z_c_z - a_x_c_x * d_z ) +
                   a_z_b_z * ( a_x_c_x * d_y - a_y_c_y * d_x );

    float det_beta = a_x_o_x * ( a_y_c_y * d_z - a_z_c_z * d_y ) + a_y_o_y * ( a_z_c_z * d_x - a_x_c_x * d_z ) +
                      a_z_o_z * ( a_x_c_x * d_y - a_y_c_y * d_x );
    float det_alpha = a_x_b_x * ( a_y_o_y * d_z - a_z_o_z * d_y ) + a_y_b_y * ( a_z_o_z * d_x - a_x_o_x * d_z ) +
                       a_z_b_z * ( a_x_o_x * d_y - a_y_o_y * d_x );
    float det_t =
            a_x_b_x * ( a_y_c_y * a_z_o_z - a_z_c_z * a_y_o_y ) + a_y_b_y * ( a_z_c_z * a_x_o_x - a_x_c_x * a_z_o_z ) +
            a_z_b_z * ( a_x_c_x * a_y_o_y - a_y_c_y * a_x_o_x );

    float beta = det_beta / det_A;
    float alpha = det_alpha / det_A;
    float t = det_t / det_A;

    if ( ( ( beta + alpha ) <= 1 ) && 0 <= beta && 0 <= alpha )
    {
        hitInfo.Parameter = t;
        hitInfo.Position = ray._origin + ray._direction * t;
        hitInfo.Normal = this->normal;

        return true;
    }

    return false;

/*    float varA = this->Vid1._data[0] - this->Vid2._data[0];
    float varB = this->Vid1._data[1] - this->Vid2._data[1];
    float varC = this->Vid1._data[2] - this->Vid2._data[2];
    float varD = this->Vid1._data[0] - this->Vid3._data[0];
    float varE = this->Vid1._data[1] - this->Vid3._data[1];
    float varF = this->Vid1._data[2] - this->Vid3._data[2];
    float varG = ray._direction._data[0];
    float varH = ray._direction._data[1];
    float varI = ray._direction._data[2];
    float varJ = this->Vid1._data[0] - ( ray._origin )._data[0];
    float varK = this->Vid1._data[1] - ( ray._origin )._data[1];
    float varL = this->Vid1._data[2] - ( ray._origin )._data[2];


    float divisor = varA * ( ( varE * varI ) - ( varH * varF ) ) + varB * ( ( varG * varF ) - ( varD * varI ) ) +
                    varC * ( ( varD * varH ) - ( varE * varG ) );
    float beta = ( varJ * ( ( varE * varI ) - ( varH * varF ) ) + varK * ( ( varG * varF ) - ( varD * varI ) ) +
                   varL * ( ( varD * varH ) - ( varE * varG ) ) ) / divisor;
    float gamma = ( varI * ( ( varA * varK ) - ( varJ * varB ) ) + varH * ( ( varJ * varC ) - ( varA * varL ) ) +
                    varG * ( ( varB * varL ) - ( varK * varC ) ) ) / divisor;

    float tval;

    if ( ( beta + gamma ) <= 1 )
    {
        if ( beta >= 0 )
        {
            if ( gamma >= 0 )
            {
                tval = ( ( ( varA * varE * varL ) + ( varB * varF * varJ ) + ( varC * varD * varK ) ) -
                         ( ( varJ * varE * varC ) + ( varK * varF * varA ) + ( varL * varD * varB ) ) ) / divisor;

                hitInfo.Parameter = tval;
                hitInfo.Position = ( ray._origin + ( ray._direction * tval ) );
                hitInfo.Normal = this->normal;

                return true;
            }
        }
    }

    return false;*/
}