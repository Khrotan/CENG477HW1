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

            float t_min = std::numeric_limits<float>::max();

            Ray ray( this->_position, d );
            RayHitInfo dummyRayHitInfo;
            RayHitInfo closestRayHitInfo;

            for ( int k = 0 ; k < CurrentScene->_meshes.size() ; k++ )
            {
                for ( int l = 0 ; l < CurrentScene->_meshes[k].triangles.size() ; l++ )
                {
                    Triangle triangle = CurrentScene->_meshes[k].triangles[l];
                    if ( triangle.Intersect(ray, dummyRayHitInfo) == true && dummyRayHitInfo.Parameter < t_min && 0.000000f < dummyRayHitInfo.Parameter )
                    {
                        t_min = dummyRayHitInfo.Parameter;
                        closestRayHitInfo = dummyRayHitInfo;
                        closestRayHitInfo.Material = CurrentScene->_meshes[k].materialId;

                        closestRayHitInfo.meshId = k;
                        closestRayHitInfo.triangleId = l;

                        closestRayHitInfo.sphereId = -1;
                    }
                }
            }

            for ( int k = 0 ; k < CurrentScene->_spheres.size() ; k++ )
            {
                Sphere sphere = CurrentScene->_spheres[k];

                if ( sphere.Intersect( ray, dummyRayHitInfo ) == true && dummyRayHitInfo.Parameter < t_min && 0.000000f < dummyRayHitInfo.Parameter )
                {
                    t_min = dummyRayHitInfo.Parameter;
                    closestRayHitInfo = dummyRayHitInfo;
                    closestRayHitInfo.Material = sphere.materialId;

                    closestRayHitInfo.meshId = -1;
                    closestRayHitInfo.triangleId = -1;

                    closestRayHitInfo.sphereId = k;
                }
            }

            if ( t_min == std::numeric_limits<float>::max() )
            {
                continue;
            }

            /*RayHitInfo closestRayHitInfo;
            Ray ray;

            bool isIntersected = this->findIntersection( i, j, closestRayHitInfo, ray );

            if ( isIntersected == false )
            {
                continue;
            }*/

            //ambient
            outputImg.Pixel( j, i ) = CurrentScene->_ambientLight *
                                      CurrentScene->_materials[closestRayHitInfo.Material].ambientCoefficient;


            for ( const auto& light : CurrentScene->_lights )
            {
                Color shadeColor = this->shade( light, closestRayHitInfo, ray, 0 );

                outputImg.Pixel( j, i )._channels[0] += shadeColor._channels[0];
                outputImg.Pixel( j, i )._channels[1] += shadeColor._channels[1];
                outputImg.Pixel( j, i )._channels[2] += shadeColor._channels[2];

/*                Color diffuseColor, billyPhongColor;
                Vector3 incomingRadiance;

                //shadow
                Vector3 w_i = ( light.position - closestRayHitInfo.Position ).normalize();

                bool isInShadow = false;
                RayHitInfo shadowHitInfo;
                Ray shadowRay( closestRayHitInfo.Position + ( (light.position - closestRayHitInfo.Position).normalize() )*0.0001f, w_i );

                for ( int k = 0 ; k < CurrentScene->_meshes.size() ; k++ )
                {
                    for ( int l = 0 ; l < CurrentScene->_meshes[k].triangles.size() ; l++ )
                    {
                        if ( closestRayHitInfo.meshId == k && closestRayHitInfo.triangleId == l ) {
                            continue;
                        }
                        if ( CurrentScene->_meshes[k].triangles[l].Intersect( shadowRay, shadowHitInfo ) == true )
                        {
                            if ( shadowHitInfo.Parameter > 0 && ( light.position - closestRayHitInfo.Position ).getLength() > ( shadowHitInfo.Position - closestRayHitInfo.Position ).getLength() ) {
                                isInShadow = true;
                                break;
                            }
                        }
                    }
                }

                for ( const auto& sphere : CurrentScene->_spheres )
                {
                    if ( sphere.Intersect( shadowRay, shadowHitInfo ) == true )
                    {
                        if ( ( light.position - closestRayHitInfo.Position ).getLength() > ( shadowHitInfo.Position - closestRayHitInfo.Position ).getLength() )
                            isInShadow = true;
                        break;
                    }
                }

                if ( isInShadow == true )
                {
                    continue;
                }

                //diffuse
                float cos_theta_prime = max( 0.0, w_i.dotProduct( closestRayHitInfo.Normal ) );

                incomingRadiance = light.intensity /
                                   ( 4 * M_PI * ( closestRayHitInfo.Position - light.position ).getLength() *
                                     ( closestRayHitInfo.Position - light.position ).getLength() );

                diffuseColor = incomingRadiance * cos_theta_prime *
                               CurrentScene->_materials[closestRayHitInfo.Material].diffuseCoefficient;

                outputImg.Pixel( j, i )._channels[0] += diffuseColor._channels[0];
                outputImg.Pixel( j, i )._channels[1] += diffuseColor._channels[1];
                outputImg.Pixel( j, i )._channels[2] += diffuseColor._channels[2];

                //billy phong
                Vector3 half_vector = ( w_i + ( this->_position - closestRayHitInfo.Position ).normalize() ).normalize();
                float cos_alpha_prime = max( 0.0, half_vector.dotProduct( closestRayHitInfo.Normal ) );

                cos_alpha_prime = pow( cos_alpha_prime,
                                       CurrentScene->_materials[closestRayHitInfo.Material].specExp );

                billyPhongColor = incomingRadiance * cos_alpha_prime *
                                  CurrentScene->_materials[closestRayHitInfo.Material].specularCoefficient;

                outputImg.Pixel( j, i )._channels[0] += billyPhongColor._channels[0];
                outputImg.Pixel( j, i )._channels[1] += billyPhongColor._channels[1];
                outputImg.Pixel( j, i )._channels[2] += billyPhongColor._channels[2];*/
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

Color Camera::shade( PointLight light, RayHitInfo closestRayHitInfo, Ray ray, int recursionDepth ) const
{
    Color diffuseColor, billyPhongColor, reflectColor, outputColor;
    Vector3 incomingRadiance;

    //shadow
    Vector3 w_i = ( light.position - closestRayHitInfo.Position ).normalize();

    bool isInShadow = false;
    RayHitInfo shadowHitInfo;
    Ray shadowRay(
            closestRayHitInfo.Position + ( ( light.position - closestRayHitInfo.Position ).normalize() ) * 0.0001f,
            w_i );

    for ( int k = 0 ; k < CurrentScene->_meshes.size() ; k++ )
    {
        for ( int l = 0 ; l < CurrentScene->_meshes[k].triangles.size() ; l++ )
        {
            if ( closestRayHitInfo.meshId == k && closestRayHitInfo.triangleId == l )
            {
                continue;
            }
            if ( CurrentScene->_meshes[k].triangles[l].Intersect( shadowRay, shadowHitInfo ) == true )
            {
                if ( shadowHitInfo.Parameter > 0 && ( light.position - closestRayHitInfo.Position ).getLength() >
                                                    ( shadowHitInfo.Position -
                                                      closestRayHitInfo.Position ).getLength() )
                {
                    isInShadow = true;
                    break;
                }
            }
        }
    }

    for ( const auto& sphere : CurrentScene->_spheres )
    {
        if ( sphere.Intersect( shadowRay, shadowHitInfo ) == true )
        {
            if ( ( light.position - closestRayHitInfo.Position ).getLength() >
                 ( shadowHitInfo.Position - closestRayHitInfo.Position ).getLength() )
            {
                isInShadow = true;
            }
            break;
        }
    }

    if ( isInShadow == true )
    {
        return Color();
    }

    //diffuse
    float cos_theta_prime = max( 0.0, w_i.dotProduct( closestRayHitInfo.Normal ) );

    incomingRadiance = light.intensity /
                       ( 4 * M_PI * ( closestRayHitInfo.Position - light.position ).getLength() *
                         ( closestRayHitInfo.Position - light.position ).getLength() );

    diffuseColor = incomingRadiance * cos_theta_prime *
                   CurrentScene->_materials[closestRayHitInfo.Material].diffuseCoefficient;

    //billy phong
    Vector3 half_vector = ( w_i + ( this->_position - closestRayHitInfo.Position ).normalize() ).normalize();
    float cos_alpha_prime = max( 0.0, half_vector.dotProduct( closestRayHitInfo.Normal ) );

    cos_alpha_prime = pow( cos_alpha_prime,
                           CurrentScene->_materials[closestRayHitInfo.Material].specExp );

    billyPhongColor = incomingRadiance * cos_alpha_prime *
                      CurrentScene->_materials[closestRayHitInfo.Material].specularCoefficient;

    if ( recursionDepth < CurrentScene->_rayReflect )
    {
        if ( CurrentScene->_materials[closestRayHitInfo.Material].reflectance.isShiny() == true )
        {
            Vector3 reflectDirection =
                    closestRayHitInfo.Normal * ( ( -2 ) * ( ray._direction.dotProduct( closestRayHitInfo.Normal ) ) ) +
                    ray._direction;
            //reflectDirection = reflectDirection.normalize();

            Ray reflectRay( closestRayHitInfo.Position, reflectDirection );

            reflectColor = reflectColor + shade( light, closestRayHitInfo, reflectRay, recursionDepth + 1 );
        }
    }

    outputColor = billyPhongColor + diffuseColor + reflectColor;

    return outputColor;
}

bool Camera::findIntersection( int i, int j, RayHitInfo& closestRayHitInfo, Ray& ray ) const
{
    float s_u = ( _imagePlane.Right - _imagePlane.Left ) * ( i + 0.5 ) / _imagePlane.Width;
    float s_v = ( _imagePlane.Top - _imagePlane.Bottom ) * ( j + 0.5 ) / _imagePlane.Height;

    Vector3 m = this->_position + this->_gaze * this->_imagePlane.Distance;
    Vector3 q = m + this->_space.Left * this->_imagePlane.Left + this->_space.Up * this->_imagePlane.Top;
    Vector3 s = q + this->_space.Left * s_u - this->_space.Up * s_v;

    Vector3 d = s - this->_position;

    float t_min = std::numeric_limits<float>::max();

    RayHitInfo dummyRayHitInfo;

    for ( int k = 0 ; k < CurrentScene->_meshes.size() ; k++ )
    {
        for ( int l = 0 ; l < CurrentScene->_meshes[k].triangles.size() ; l++ )
        {
            Triangle triangle = CurrentScene->_meshes[k].triangles[l];
            if ( triangle.Intersect( ray, dummyRayHitInfo ) == true && dummyRayHitInfo.Parameter < t_min &&
                 0.000000f < dummyRayHitInfo.Parameter )
            {
                t_min = dummyRayHitInfo.Parameter;
                closestRayHitInfo = dummyRayHitInfo;
                closestRayHitInfo.Material = CurrentScene->_meshes[k].materialId;

                closestRayHitInfo.meshId = k;
                closestRayHitInfo.triangleId = l;

                closestRayHitInfo.sphereId = -1;
            }
        }
    }

    for ( int k = 0 ; k < CurrentScene->_spheres.size() ; k++ )
    {
        Sphere sphere = CurrentScene->_spheres[k];

        if ( sphere.Intersect( ray, dummyRayHitInfo ) == true && dummyRayHitInfo.Parameter < t_min &&
             0.000000f < dummyRayHitInfo.Parameter )
        {
            t_min = dummyRayHitInfo.Parameter;
            closestRayHitInfo = dummyRayHitInfo;
            closestRayHitInfo.Material = sphere.materialId;

            closestRayHitInfo.meshId = -1;
            closestRayHitInfo.triangleId = -1;

            closestRayHitInfo.sphereId = k;
        }
    }

    if ( t_min == std::numeric_limits<float>::max() )
    {
        return false;
    }
    else
    {
        return true;
    }
}
