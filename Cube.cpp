//
// Created by Arda on 16.11.2016.
//

#include "Cube.h"

Cube::Cube() :
        vec0( Vector3( 0.5, 0.5, 0.5 ) ),
        vec1( Vector3( 0.5, -0.5, 0.5 ) ),
        vec2( Vector3( -0.5, 0.5, 0.5 ) ),
        vec3( Vector3( -0.5, -0.5, 0.5 ) ),
        vec4( Vector3( 0.5, 0.5, -0.5 ) ),
        vec5( Vector3( 0.5, -0.5, -0.5 ) ),
        vec6( Vector3( -0.5, 0.5, -0.5 ) ),
        vec7( Vector3( -0.5, -0.5, -0.5 ) ),
        up_1( Triangle( vec6, vec2, vec0 ) ),
        up_2( Triangle( vec6, vec0, vec4 ) ),
        bottom_1( Triangle( vec1, vec3, vec5 ) ),
        bottom_2( Triangle( vec7, vec5, vec3 ) ),
        left_1( Triangle( vec7, vec3, vec2 ) ),
        left_2( Triangle( vec2, vec6, vec7 ) ),
        right_1( Triangle( vec0, vec1, vec4 ) ),
        right_2( Triangle( vec5, vec4, vec1 ) ),
        front_1( Triangle( vec2, vec1, vec0 ) ),
        front_2( Triangle( vec3, vec1, vec2 ) ),
        back_1( Triangle( vec4, vec5, vec6 ) ),
        back_2( Triangle( vec7, vec6, vec5 ) )
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
