//
// Created by Arda on 26.10.2016.
//

#include "Sphere.h"

bool Sphere::Intersect( const Ray& ray, RayHitInfo& hitInfo ) const
{
    if ( ray._direction.dotProduct( ray._direction ) == 0 )
    {
        //TODO ????
        return false;
    }

    double delta = ( ray._origin - this->center ).dotProduct( ray._direction );
    delta *= delta;
    delta -= ( ray._direction.dotProduct( ray._direction ) ) *
             ( ( ray._origin - this->center ).dotProduct( ray._origin - this->center ) - radius * radius );
    delta = sqrt( delta );

    double d_d = ray._direction.dotProduct( ray._direction );

    double t_1 = -( ray._origin - this->center ).dotProduct( ray._direction );
    t_1 = ( t_1 + delta ) / d_d;

    double t_2 = -( ray._origin - this->center ).dotProduct( ray._direction );
    t_2 = ( t_2 - delta ) / d_d;


    if ( t_1 > 0 && t_2 > 0 )
    {
        hitInfo.Material = this->materialId;

        if ( t_1 < t_2 )
        {
            hitInfo.Position = ray._origin + ray._direction * t_1;
            hitInfo.Parameter = t_1;
        }
        else
        {
            hitInfo.Position = ray._origin + ray._direction * t_2;
            hitInfo.Parameter = t_2;
        }

        hitInfo.Normal = ( ( hitInfo.Position - this->center ) / this->radius ).normalize();

        return true;
    }

    return false;
}

Sphere::Sphere() : radius( 1 ),
                   center( Vector3( 0, 0, 0 ) )
{

}

void Sphere::applyScaling( Scaling scaling )
{
    this->radius *= scaling.s_x;
}

void Sphere::applyTranlation( Translation translation )
{
    this->center._data[0] += translation.t_x;
    this->center._data[1] += translation.t_y;
    this->center._data[2] += translation.t_z;
}
