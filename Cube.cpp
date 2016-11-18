//
// Created by Arda on 16.11.2016.
//

#include <math.h>
#include "Cube.h"

Cube::Cube() :
        solArkaUst( Vector3( -0.5, 0.5, -0.5 ) ),
        solOnUst( Vector3( -0.5, 0.5, 0.5 ) ),
        sagOnUst( Vector3( 0.5, 0.5, 0.5 ) ),
        sagArkaUst( Vector3( 0.5, 0.5, -0.5 ) ),
        solArkaAlt( Vector3( -0.5, -0.5, -0.5 ) ),
        solOnAlt( Vector3( -0.5, -0.5, 0.5 ) ),
        sagOnAlt( Vector3( 0.5, -0.5, 0.5 ) ),
        sagArkaAlt( Vector3( 0.5, -0.5, -0.5 ) ),
        up_1( Triangle( solOnUst, sagArkaUst, solArkaUst ) ),
        up_2( Triangle( solOnUst, sagArkaUst, sagOnUst ) ),
        bottom_1( Triangle( solArkaAlt, sagArkaAlt, solOnAlt ) ),
        bottom_2( Triangle( sagArkaAlt, sagOnAlt, solOnAlt ) ),
        left_1( Triangle( solArkaUst, solOnAlt, solOnUst ) ),
        left_2( Triangle( solArkaUst, solArkaAlt, solOnAlt ) ),
        right_1( Triangle( sagArkaUst, sagArkaAlt, sagOnUst ) ),
        right_2( Triangle( sagArkaAlt, sagOnAlt, sagOnUst ) ),
        front_1( Triangle( sagOnUst, sagOnAlt, solOnAlt ) ),
        front_2( Triangle( solOnUst, sagOnUst, solOnAlt ) ),
        back_1( Triangle( sagArkaUst, sagArkaAlt, solArkaAlt ) ),
        back_2( Triangle( solArkaUst, sagArkaUst, solArkaAlt ) )
{
    this->triangles.push_back( up_1 );
    this->triangles.push_back( up_2 );
    this->triangles.push_back( bottom_1 );
    this->triangles.push_back( bottom_2 );
    this->triangles.push_back( left_1 );
    this->triangles.push_back( left_2 );
    this->triangles.push_back( right_1 );
    this->triangles.push_back( right_2 );
    this->triangles.push_back( front_1 );
    this->triangles.push_back( front_2 );
    this->triangles.push_back( back_1 );
    this->triangles.push_back( back_2 );
}

void Cube::applyScaling( Scaling scaling )
{
    for ( auto& triangle : this->triangles )
    {
        triangle.Vid1._data[0] *= scaling.s_x;
        triangle.Vid2._data[0] *= scaling.s_x;
        triangle.Vid3._data[0] *= scaling.s_x;

        triangle.Vid1._data[1] *= scaling.s_y;
        triangle.Vid2._data[1] *= scaling.s_y;
        triangle.Vid3._data[1] *= scaling.s_y;

        triangle.Vid1._data[2] *= scaling.s_z;
        triangle.Vid2._data[2] *= scaling.s_z;
        triangle.Vid3._data[2] *= scaling.s_z;
    }
}

void Cube::applyTranslation( Translation translation )
{
    for ( auto& triangle : this->triangles )
    {
        triangle.Vid1._data[0] += translation.t_x;
        triangle.Vid2._data[0] += translation.t_x;
        triangle.Vid3._data[0] += translation.t_x;

        triangle.Vid1._data[1] += translation.t_y;
        triangle.Vid2._data[1] += translation.t_y;
        triangle.Vid3._data[1] += translation.t_y;

        triangle.Vid1._data[2] += translation.t_z;
        triangle.Vid2._data[2] += translation.t_z;
        triangle.Vid3._data[2] += translation.t_z;
    }
}
