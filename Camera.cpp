//
// Created by Arda on 26.10.2016.
//

#include <limits>
#include "Camera.h"
#include "Sphere.h"
#include "Mesh.h"
#include "Scene.h"

#define M_PI 3.14159265358979323846

Image Camera::Render() const
{
    Image outputImg( this->_imagePlane.Width, this->_imagePlane.Height, CurrentScene->_backgroundColour );

    for ( int i = 0 ; i < this->_imagePlane.Width ; i++ )
    {
        for ( int j = 0 ; j < this->_imagePlane.Height ; j++ )
        {
            float s_u = ( _imagePlane.Right - _imagePlane.Left ) * ( i + 0.5 ) / _imagePlane.Width;
            float s_v = ( _imagePlane.Top - _imagePlane.Bottom ) * ( j + 0.5 ) / _imagePlane.Height;

            Vector3 m = this->_position + this->_gaze * this->_imagePlane.Distance;
            Vector3 q = m + this->_space.Left * this->_imagePlane.Left + this->_space.Up * this->_imagePlane.Top;
            Vector3 s = q + this->_space.Left * s_u - this->_space.Up * s_v;

            Vector3 d = s - this->_position;

            float t_min_sphere = std::numeric_limits<float>::max();
            float t_min_mesh = std::numeric_limits<float>::max();

            Sphere closestSphere;
            Mesh closestMesh;

            Ray ray( this->_position, d );
            RayHitInfo rayHitInfo;
            RayHitInfo closestRayHitInfo;

            for ( const auto& mesh : CurrentScene->_meshes )
            {
                for ( const auto& triangle : mesh.triangles )
                {
                    if ( triangle.Intersect(ray, rayHitInfo) == true && rayHitInfo.Parameter < t_min_mesh )
                    {
                        t_min_mesh = rayHitInfo.Parameter;
                        closestMesh = mesh;
                        closestRayHitInfo = rayHitInfo;
                    }
                }
            }

            for ( const auto& sphere : CurrentScene->_spheres )
            {
                if ( sphere.Intersect( ray, rayHitInfo ) == true && rayHitInfo.Parameter < t_min_sphere )
                {
                    t_min_sphere = rayHitInfo.Parameter;
                    closestSphere = sphere;
                    closestRayHitInfo = rayHitInfo;
                }
            }

            if ( t_min_sphere == std::numeric_limits<float>::max() &&
                 t_min_mesh == std::numeric_limits<float>::max() )
            {
                continue;
            }


            //ambient
            if ( t_min_sphere < t_min_mesh )
            {
                outputImg.Pixel( j, i ) = CurrentScene->_ambientLight *
                                          CurrentScene->_materials[closestSphere.materialId].ambientCoefficient;
            }
            else
            {
                outputImg.Pixel( j, i ) = CurrentScene->_ambientLight *
                                          CurrentScene->_materials[closestMesh.materialId].ambientCoefficient;
            }

            for ( const auto& light : CurrentScene->_lights )
            {
                Color diffuseColor, billyPhongColor;
                Vector3 incomingRadiance;

                //shadow
                bool isInShadow = false;
                RayHitInfo shadowHitInfo;
                Ray shadowRay( closestRayHitInfo.Position, light.position - closestRayHitInfo.Position );
                for ( const auto& sphere : CurrentScene->_spheres )
                {
/*                    if ( &sphere == &closestSphere ) {
                        continue;
                    }*/

                    if ( sphere.Intersect( shadowRay, shadowHitInfo ) == true )
                    {
                        isInShadow = true;
                        break;
                    }
                }

/*                for ( const auto& mesh : CurrentScene->_meshes )
                {
                    if ( &sphere == &closestSphere ) {
                        continue;
                    }

                    for ( const auto& triangle : mesh.triangles )
                    {
                        if ( triangle.Intersect( shadowRay, shadowHitInfo ) == true )
                        {
                            isInShadow = true;
                            break;
                        }
                    }
                }*/

                if ( isInShadow == true )
                {
                    continue;
                }

                //diffuse
                Vector3 w_i = ( light.position - closestRayHitInfo.Position ).normalize();
                float cos_theta_prime = max( 0.0, w_i.dotProduct( closestRayHitInfo.Normal ) );

                incomingRadiance = light.intensity /
                                   ( 4 * M_PI * ( closestRayHitInfo.Position - light.position ).getLength() *
                                     ( closestRayHitInfo.Position - light.position ).getLength() );

                if ( t_min_sphere < t_min_mesh )
                {
                    diffuseColor = incomingRadiance * cos_theta_prime *
                                   CurrentScene->_materials[closestSphere.materialId].diffuseCoefficient;
                }
                else
                {
                    diffuseColor = incomingRadiance * cos_theta_prime *
                                   CurrentScene->_materials[closestMesh.materialId].diffuseCoefficient;
                }

                outputImg.Pixel( j, i )._channels[0] += diffuseColor._channels[0];
                outputImg.Pixel( j, i )._channels[1] += diffuseColor._channels[1];
                outputImg.Pixel( j, i )._channels[2] += diffuseColor._channels[2];

                //billy phong
                Vector3 half_vector = ( w_i + ( this->_position - closestRayHitInfo.Position ).normalize() ).normalize();
                float cos_alpha_prime = max( 0.0, half_vector.dotProduct( closestRayHitInfo.Normal ) );

                if ( t_min_sphere < t_min_mesh )
                {
                    cos_alpha_prime = pow( cos_alpha_prime,
                                           CurrentScene->_materials[closestSphere.materialId].specExp );
                }
                else
                {
                    cos_alpha_prime = pow( cos_alpha_prime,
                                           CurrentScene->_materials[closestMesh.materialId].specExp );
                }

                if ( t_min_sphere < t_min_mesh )
                {
                    billyPhongColor = incomingRadiance * cos_alpha_prime *
                                      CurrentScene->_materials[closestSphere.materialId].specularCoefficient;
                }
                else
                {
                    billyPhongColor = incomingRadiance * cos_alpha_prime *
                                      CurrentScene->_materials[closestMesh.materialId].specularCoefficient;
                }

                outputImg.Pixel( j, i )._channels[0] += billyPhongColor._channels[0];
                outputImg.Pixel( j, i )._channels[1] += billyPhongColor._channels[1];
                outputImg.Pixel( j, i )._channels[2] += billyPhongColor._channels[2];
            }

            if ( outputImg.Pixel( j, i )._channels[0] > 255 )
            {
                outputImg.Pixel( j, i )._channels[0] = 255;
            }
            if ( outputImg.Pixel( j, i )._channels[1] > 255 )
            {
                outputImg.Pixel( j, i )._channels[1] = 255;
            }
            if ( outputImg.Pixel( j, i )._channels[2] > 255 )
            {
                outputImg.Pixel( j, i )._channels[2] = 255;
            }

            if ( outputImg.Pixel( j, i )._channels[0] < 0 )
            {
                outputImg.Pixel( j, i )._channels[0] = 0;
            }
            if ( outputImg.Pixel( j, i )._channels[1] < 0 )
            {
                outputImg.Pixel( j, i )._channels[1] = 0;
            }
            if ( outputImg.Pixel( j, i )._channels[2] < 0 )
            {
                outputImg.Pixel( j, i )._channels[2] = 0;
            }
        }
    }


    return outputImg;
}
