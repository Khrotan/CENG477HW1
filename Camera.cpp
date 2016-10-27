//
// Created by Arda on 26.10.2016.
//

#include <limits>
#include "Camera.h"
#include "Sphere.h"
#include "Mesh.h"
#include "Scene.h"

Image Camera::Render() const
{
    Image outputImg( this->_imagePlane.Width, this->_imagePlane.Height, CurrentScene->_backgroundColour );

    for ( int i = 0 ; i < this->_imagePlane.Width ; i++ ) {
        for ( int j = 0 ; j < this->_imagePlane.Height ; j++ ) {
            double s_u = ( _imagePlane.Right - _imagePlane.Left ) * ( ((double) j ) + 0.5 ) / (_imagePlane.Width);
            double s_v = ( _imagePlane.Right - _imagePlane.Left ) * ( ((double) i ) + 0.5 ) / (_imagePlane.Height);

            Vector3 m = this->_position + this->_gaze * this->_imagePlane.Distance;
            Vector3 q = m + this->_space.Left * this->_imagePlane.Left + this->_space.Up * this->_imagePlane.Top;
            Vector3 s = q + this->_space.Left * s_u - this->_space.Up * s_v;

            Vector3 d = s - this->_position;

            double t_min_sphere = std::numeric_limits<double>::max();
            double t_min_mesh = std::numeric_limits<double>::max();

            Sphere closestSphere;
            Mesh closestMesh;

            Ray ray( this->_position, d );
            RayHitInfo rayHitInfo;

            for ( const auto& sphere : CurrentScene->_spheres ) {
                if ( sphere.Intersect(ray, rayHitInfo) == true && rayHitInfo.Parameter < t_min_sphere ) {
                    t_min_sphere = rayHitInfo.Parameter;
                    closestSphere = sphere;
                }
            }

            /*for ( const auto& mesh : CurrentScene->_meshes ) {
                RayHitInfo rayHitInfo;
                if ( mesh.Intersect(ray, rayHitInfo) == true && rayHitInfo.Parameter < t_min_mesh ) {
                    t_min_mesh = rayHitInfo.Parameter;
                    closestMesh = mesh;
                }
            }*/

            if ( t_min_sphere < t_min_mesh ) {
                //ambient
                outputImg.Pixel( i,j ) = CurrentScene->_ambientLight * CurrentScene->_materials[closestSphere.materialId].ambientCoefficient ;
                for ( const auto& light : CurrentScene->_lights ) {
                    //shadow
                    bool isInShadow = false;
                    RayHitInfo shadowHitInfo;
                    Ray shadowRay( rayHitInfo.Position, light.position - rayHitInfo.Position );
                    for ( const auto& sphere : CurrentScene->_spheres ) {
                        if ( &sphere == &closestSphere ) {
                            continue;
                        }
                        if ( sphere.Intersect( shadowRay, shadowHitInfo ) == true ) {
                            isInShadow = true;
                            break;
                        }
                    }

                    if (  isInShadow == false )
                    {
                        //diffuse
                        Vector3 w_i = ( light.position - rayHitInfo.Position ).normalize();

                        double cos_theta_prime = max(0.0, w_i.dotProduct( rayHitInfo.Normal ));

                        Color incomingRadiance = light.intensity / ( ( rayHitInfo.Position - light.position ).getLength() * ( rayHitInfo.Position - light.position ).getLength() );

                        Color diffuseColor =  incomingRadiance * cos_theta_prime * CurrentScene->_materials[closestSphere.materialId].diffuseCoefficient;

                        outputImg.Pixel( i,j )._channels[0] += diffuseColor._channels[0];
                        outputImg.Pixel( i,j )._channels[1] += diffuseColor._channels[1];
                        outputImg.Pixel( i,j )._channels[2] += diffuseColor._channels[2];

                        //billy phong
                        Vector3 half_vector = ( w_i + this->_space.Forward ).normalize();

                        double cos_alpha_prime = max(0.0, half_vector.dotProduct( rayHitInfo.Normal ));
                        cos_alpha_prime = pow( cos_alpha_prime, CurrentScene->_materials[closestSphere.materialId].specExp );

                        Color billyPhongColor = incomingRadiance * cos_alpha_prime * CurrentScene->_materials[closestSphere.materialId].specularCoefficient;

                        outputImg.Pixel( i,j )._channels[0] += billyPhongColor._channels[0];
                        outputImg.Pixel( i,j )._channels[1] += billyPhongColor._channels[1];
                        outputImg.Pixel( i,j )._channels[2] += billyPhongColor._channels[2];
                    }


                }
            }

            if ( outputImg.Pixel( i, j )._channels[0] > 255 ) {
                outputImg.Pixel( i, j )._channels[0] = 255;
            }
            if ( outputImg.Pixel( i, j )._channels[1] > 255 ) {
                outputImg.Pixel( i, j )._channels[1] = 255;
            }
            if ( outputImg.Pixel( i, j )._channels[2] > 255 ) {
                outputImg.Pixel( i, j )._channels[2] = 255;
            }
        }
    }

    return outputImg;
}
