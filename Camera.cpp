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
    Image outputImg( this->_imagePlane.Width, this->_imagePlane.Height, CurrentScene->_background );

    for ( int i = 0 ; i < this->_imagePlane.Width ; i++ ) {
        for ( int j = 0 ; j < this->_imagePlane.Height ; j++ ) {
            double s_u = ( _imagePlane.Right - _imagePlane.Left ) * ( ((double) i ) + 0.5 ) / (_imagePlane.Width);
            double s_v = ( _imagePlane.Right - _imagePlane.Left ) * ( ((double) j ) + 0.5 ) / (_imagePlane.Height);

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
                outputImg.Pixel( i,j ) = CurrentScene->_materials[closestSphere.materialId].ambient;
                for ( const auto& light : CurrentScene->_lights ) {
                    //diffuse
                    Vector3 w_i = ( light.position - rayHitInfo.Position );
                    w_i.normalize();

                    double cos_theta_prime = max(0.0, w_i.dotProduct( rayHitInfo.Normal ));

                    //Color color = CurrentScene->_materials[closestSphere.materialId].diffuse * cos_theta_prime * light.intensity
                    //Color diffuseColor = CurrentScene->_materials[closestSphere.materialId].diffuse * cos_theta_prime;

                    double radiance = ( cos_theta_prime / ( light.position - rayHitInfo.Position ).getLength() );

                    Color diffuseColor = light.intensity * radiance;

                    outputImg.Pixel( i,j )._channels[0] += diffuseColor._channels[0];
                    outputImg.Pixel( i,j )._channels[1] += diffuseColor._channels[1];
                    outputImg.Pixel( i,j )._channels[2] += diffuseColor._channels[2];

                    //billy phong
                    Vector3 half_vector = ( w_i + this->_space.Forward );
                    half_vector.normalize();

                    double cos_alpha_prime = max(0.0, half_vector.dotProduct( rayHitInfo.Normal ));
                    cos_alpha_prime = pow( cos_alpha_prime, CurrentScene->_materials[closestSphere.materialId].specExp );

                    radiance = ( cos_alpha_prime / ( light.position - rayHitInfo.Position ).getLength() );

                    Color billyPhongColor = light.intensity * radiance;

                    outputImg.Pixel( i,j )._channels[0] += billyPhongColor._channels[0];
                    outputImg.Pixel( i,j )._channels[1] += billyPhongColor._channels[1];
                    outputImg.Pixel( i,j )._channels[2] += billyPhongColor._channels[2];
                }
            }
        }
    }

    return outputImg;
}
