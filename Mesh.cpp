//
// Created by Arda on 29.10.2016.
//

#include <limits>
#include "Ray.h"
#include "Mesh.h"

bool Mesh::Intersect( const Ray& ray, RayHitInfo& hitInfo ) const
{
    float t_min_triangle = std::numeric_limits<float>::max();

    for ( const auto& triangle : this->triangles ) {
        if ( triangle.Intersect( ray, hitInfo ) == true ) {
            if ( hitInfo.Parameter < t_min_triangle ) {
                t_min_triangle = hitInfo.Parameter;
            }
        }
    }

    if ( t_min_triangle < std::numeric_limits<float>::max() ) {
        return true;
    }
    return false;
}
