//
// Created by Arda on 26.10.2016.
//

#include <limits>
#include <math.h>
#include "Camera.h"
#include "Scene.h"

Image Camera::Render() const
{
    Image outputImg(this->_imagePlane.Width, this->_imagePlane.Height, CurrentScene->_backgroundColour);

    for ( int i = 0 ; i < this->_imagePlane.Width ; i++ )
    {
        for ( int j = 0 ; j < this->_imagePlane.Height ; j++ )
        {
            double s_u = (_imagePlane.Right - _imagePlane.Left) * (i + 0.5) / _imagePlane.Width;
            double s_v = (_imagePlane.Top - _imagePlane.Bottom) * (j + 0.5) / _imagePlane.Height;

            Vector3 m = this->_position + this->_gaze * this->_imagePlane.Distance;
            Vector3 q = m + this->_space.Left * this->_imagePlane.Left + this->_space.Up * this->_imagePlane.Top;
            Vector3 s = q + this->_space.Left * s_u - this->_space.Up * s_v;

            Vector3 d = s - this->_position;

            Ray ray;
            ray._origin    = this->_position;
            ray._direction = d;

            RayHitInfo closestRayHitInfo;

            bool isIntersected = fillHitInfo(closestRayHitInfo, ray);

            if ( isIntersected == false )
            { continue; }

            //ambient
            outputImg.Pixel(j, i) = CurrentScene->_ambientLight *
                                    CurrentScene->_materials[closestRayHitInfo.Material].ambientCoefficient;

            for ( const auto& light : CurrentScene->_lights )
            {
                Color shadeColor = this->shade(light, closestRayHitInfo, ray, 0);

                outputImg.Pixel(j, i)._channels[0] += shadeColor._channels[0];
                outputImg.Pixel(j, i)._channels[1] += shadeColor._channels[1];
                outputImg.Pixel(j, i)._channels[2] += shadeColor._channels[2];
            }

            if ( outputImg.Pixel(j, i)._channels[0] > 255 )
            {
                outputImg.Pixel(j, i)._channels[0] = 255;
            }
            if ( outputImg.Pixel(j, i)._channels[1] > 255 )
            {
                outputImg.Pixel(j, i)._channels[1] = 255;
            }
            if ( outputImg.Pixel(j, i)._channels[2] > 255 )
            {
                outputImg.Pixel(j, i)._channels[2] = 255;
            }

            if ( outputImg.Pixel(j, i)._channels[0] < 0 )
            {
                outputImg.Pixel(j, i)._channels[0] = 0;
            }
            if ( outputImg.Pixel(j, i)._channels[1] < 0 )
            {
                outputImg.Pixel(j, i)._channels[1] = 0;
            }
            if ( outputImg.Pixel(j, i)._channels[2] < 0 )
            {
                outputImg.Pixel(j, i)._channels[2] = 0;
            }
        }
    }
    return outputImg;
}

Color Camera::shade( PointLight light, RayHitInfo closestRayHitInfo, Ray ray, int recursionDepth ) const
{
    Color   diffuseColor, billyPhongColor, reflectColor, outputColor;
    Vector3 incomingRadiance;

    //shadow
    Vector3 w_i = (light.position - closestRayHitInfo.Position).normalize();

    bool       isInShadow = false;
    RayHitInfo shadowHitInfo;
    Ray        shadowRay(
            closestRayHitInfo.Position + ((light.position - closestRayHitInfo.Position).normalize()) * 0.001,
            w_i);

    for ( int k = 0 ; k < CurrentScene->_cubes.size() ; k++ )
    {
        for ( int l = 0 ; l < CurrentScene->_cubes[k].triangles.size() ; l++ )
        {
            if ( CurrentScene->_cubes[k].triangles[l].Intersect(shadowRay, shadowHitInfo) == true )
            {
                if ( shadowHitInfo.Parameter > 0 && (light.position - closestRayHitInfo.Position).getLength() >
                                                    (shadowHitInfo.Position -
                                                     closestRayHitInfo.Position).getLength())
                {
                    isInShadow = true;
                    break;
                }
            }
        }
    }

    for ( const auto& sphere : CurrentScene->_spheres )
    {
        if ( sphere.Intersect(shadowRay, shadowHitInfo) == true )
        {
            if ((light.position - closestRayHitInfo.Position).getLength() >
                (shadowHitInfo.Position - closestRayHitInfo.Position).getLength())
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
    double cos_theta_prime = max(0.0, w_i.dotProduct(closestRayHitInfo.Normal));

    incomingRadiance = light.intensity /
                       (4 * M_PI * (closestRayHitInfo.Position - light.position).getLength() *
                        (closestRayHitInfo.Position - light.position).getLength());

    if ( closestRayHitInfo.textureId == 0 )
    {
        diffuseColor = incomingRadiance * cos_theta_prime *
                       CurrentScene->_materials[closestRayHitInfo.Material].diffuseCoefficient;
    }
    else
    {
        Color jpgColor;
        jpgColor._channels[0] = (
                (double) CurrentScene->_textures[closestRayHitInfo.textureId].image[closestRayHitInfo.i][closestRayHitInfo.j][0]);
        jpgColor._channels[1] = (
                (double) CurrentScene->_textures[closestRayHitInfo.textureId].image[closestRayHitInfo.i][closestRayHitInfo.j][1]);
        jpgColor._channels[2] = (
                (double) CurrentScene->_textures[closestRayHitInfo.textureId].image[closestRayHitInfo.i][closestRayHitInfo.j][2]);

        diffuseColor =
                CurrentScene->_materials[closestRayHitInfo.Material].diffuseCoefficient * cos_theta_prime * jpgColor;

        /*diffuseColor = incomingRadiance * cos_theta_prime *
                       CurrentScene->_materials[closestRayHitInfo.Material].diffuseCoefficient;*/
    }

    //billy phong
    Vector3 half_vector     = (w_i + (this->_position - closestRayHitInfo.Position).normalize()).normalize();
    double  cos_alpha_prime = max(0.0, half_vector.dotProduct(closestRayHitInfo.Normal));

    cos_alpha_prime = pow(cos_alpha_prime,
                          CurrentScene->_materials[closestRayHitInfo.Material].specExp);

    billyPhongColor = incomingRadiance * cos_alpha_prime *
                      CurrentScene->_materials[closestRayHitInfo.Material].specularCoefficient;

    if ( recursionDepth < CurrentScene->_rayReflect )
    {
        if ( CurrentScene->_materials[closestRayHitInfo.Material].reflectance.isShiny() == true )
        {
            Vector3 reflectDirection =
                            closestRayHitInfo.Normal * ((-2) * (ray._direction.dotProduct(closestRayHitInfo.Normal))) +
                            ray._direction;
            reflectDirection              = reflectDirection.normalize();

            Ray reflectRay(closestRayHitInfo.Position + reflectDirection * 0.0001, reflectDirection);

            RayHitInfo reflectHitInfo;
            bool       isReflectIntersect = fillHitInfo(reflectHitInfo, reflectRay);

            if ( isReflectIntersect )
            {
                reflectColor = reflectColor + shade(light, reflectHitInfo, reflectRay, recursionDepth + 1);
            }
        }
    }

    outputColor = billyPhongColor + diffuseColor + reflectColor;

    return outputColor;
}

bool Camera::fillHitInfo( RayHitInfo& closestRayHitInfo, Ray ray ) const
{
    double t_min = std::numeric_limits<double>::max();

    RayHitInfo dummyRayHitInfo;

    for ( int k = 0 ; k < CurrentScene->_cubes.size() ; k++ )
    {
        for ( int l = 0 ; l < CurrentScene->_cubes[k].triangles.size() ; l++ )
        {
            Triangle triangle = CurrentScene->_cubes[k].triangles[l];
            if ( triangle.Intersect(ray, dummyRayHitInfo) == true && dummyRayHitInfo.Parameter < t_min &&
                 0.0001 < dummyRayHitInfo.Parameter )
            {
                t_min             = dummyRayHitInfo.Parameter;
                closestRayHitInfo = dummyRayHitInfo;
                closestRayHitInfo.Material = CurrentScene->_cubes[k].materialId;

                closestRayHitInfo.meshId     = k;
                closestRayHitInfo.triangleId = l;

                closestRayHitInfo.sphereId  = -1;
                closestRayHitInfo.textureId = CurrentScene->_cubes[k].textureId;

                findI_J_cube(closestRayHitInfo);
            }
        }
    }

    for ( int k = 0 ; k < CurrentScene->_spheres.size() ; k++ )
    {
        Sphere sphere = CurrentScene->_spheres[k];

        if ( sphere.Intersect(ray, dummyRayHitInfo) == true && dummyRayHitInfo.Parameter < t_min &&
             0.0001 < dummyRayHitInfo.Parameter )
        {
            t_min             = dummyRayHitInfo.Parameter;
            closestRayHitInfo = dummyRayHitInfo;
            closestRayHitInfo.Material = sphere.materialId;

            closestRayHitInfo.meshId     = -1;
            closestRayHitInfo.triangleId = -1;

            closestRayHitInfo.sphereId  = k;
            closestRayHitInfo.textureId = CurrentScene->_spheres[k].textureId;

            findI_J(closestRayHitInfo, sphere);
        }
    }

    if ( t_min == std::numeric_limits<double>::max())
    {
        return false;
    }
    else
    {
        return true;
    }
}

void Camera::findI_J_cube( RayHitInfo& info ) const
{
    //alpha beta gamma
        //info.j = round(ver1.X() * info.alpha + ver2.X() * info.beta + ver3.X() * info.gamma);
        //info.i = round(ver1.Y() * info.alpha + ver2.Y() * info.beta + ver3.Y() * info.gamma);
    //alpha gamma beta
        //info.j = round(ver1.X() * info.alpha + ver2.X() * info.gamma + ver3.X() * info.beta);
        //info.i = round(ver1.Y() * info.alpha + ver2.Y() * info.gamma + ver3.Y() * info.beta);
    //beta alpha gamma
        //info.j = round(ver1.X() * info.beta + ver2.X() * info.alpha + ver3.X() * info.gamma);
        //info.i = round(ver1.Y() * info.beta + ver2.Y() * info.alpha + ver3.Y() * info.gamma);
    //beta gamma alpha
        //info.j = round(ver1.X() * info.beta + ver2.X() * info.gamma + ver3.X() * info.alpha);
        //info.i = round(ver1.Y() * info.beta + ver2.Y() * info.gamma + ver3.Y() * info.alpha);
    //gamma alpha beta
        //info.j = round(ver1.X() * info.gamma + ver2.X() * info.alpha + ver3.X() * info.beta);
        //info.i = round(ver1.Y() * info.gamma + ver2.Y() * info.alpha + ver3.Y() * info.beta);
    //gamma beta alpha
        //info.j = round(ver1.X() * info.gamma + ver2.X() * info.beta + ver3.X() * info.alpha);
        //info.i = round(ver1.Y() * info.gamma + ver2.Y() * info.beta + ver3.Y() * info.alpha);

    Vector3 ver1, ver2, ver3;
    if ( info.triangleId == 0 ) //up_1 //ok?
    {
        ver1.X() = 2 * CurrentScene->_textures[info.textureId].width / 3;
        ver1.Y() = CurrentScene->_textures[info.textureId].height / 2;

        ver2.X() = CurrentScene->_textures[info.textureId].width - 1;
        ver2.Y() = CurrentScene->_textures[info.textureId].height / 2;

        ver3.X() = CurrentScene->_textures[info.textureId].width - 1;
        ver3.Y() = CurrentScene->_textures[info.textureId].height - 1;

        info.j = round(ver1.X() * info.alpha + ver2.X() * info.beta + ver3.X() * info.gamma);
        info.i = round(ver1.Y() * info.alpha + ver2.Y() * info.beta + ver3.Y() * info.gamma);
    }
    else if ( info.triangleId == 1 ) //up_2 //ok?
    {
        ver1.X() = CurrentScene->_textures[info.textureId].width - 1;
        ver1.Y() = CurrentScene->_textures[info.textureId].height - 1;

        ver2.X() = 2 * CurrentScene->_textures[info.textureId].width / 3;
        ver2.Y() = CurrentScene->_textures[info.textureId].height - 1;

        ver3.X() = 2 * CurrentScene->_textures[info.textureId].width / 3;
        ver3.Y() = CurrentScene->_textures[info.textureId].height / 2;

        info.j = round(ver1.X() * info.alpha + ver2.X() * info.beta + ver3.X() * info.gamma);
        info.i = round(ver1.Y() * info.alpha + ver2.Y() * info.beta + ver3.Y() * info.gamma);
    }
    else if ( info.triangleId == 2 ) // bottom_1
    {
        ver1.X() = 2 * CurrentScene->_textures[info.textureId].width / 3 - 1;
        ver1.Y() = 0;

        ver2.X() = CurrentScene->_textures[info.textureId].width - 1;
        ver2.Y() = 0;

        ver3.X() = CurrentScene->_textures[info.textureId].width - 1;
        ver3.Y() = CurrentScene->_textures[info.textureId].height / 2;

        info.j = round(ver1.X() * info.alpha + ver2.X() * info.beta + ver3.X() * info.gamma);
        info.i = round(ver1.Y() * info.alpha + ver2.Y() * info.beta + ver3.Y() * info.gamma);
    }
    else if ( info.triangleId == 3 ) //bottom_2
    {
        ver1.X() = CurrentScene->_textures[info.textureId].width - 1;
        ver1.Y() = CurrentScene->_textures[info.textureId].height / 2;

        ver2.X() = 2 * CurrentScene->_textures[info.textureId].width / 3;
        ver2.Y() = CurrentScene->_textures[info.textureId].height / 2;

        ver3.X() = 2 * CurrentScene->_textures[info.textureId].width / 3;
        ver3.Y() = 0;

        info.j = round(ver1.X() * info.alpha + ver2.X() * info.beta + ver3.X() * info.gamma);
        info.i = round(ver1.Y() * info.alpha + ver2.Y() * info.beta + ver3.Y() * info.gamma);
    }
    else if ( info.triangleId == 4 ) //left_1
    {
        ver1.X() = 0;
        ver1.Y() = 0;

        ver2.X() = CurrentScene->_textures[info.textureId].width / 3;
        ver2.Y() = 0;

        ver3.X() = CurrentScene->_textures[info.textureId].width / 3;
        ver3.Y() = CurrentScene->_textures[info.textureId].height / 2;

        info.j = round(ver1.X() * info.alpha + ver2.X() * info.beta + ver3.X() * info.gamma);
        info.i = round(ver1.Y() * info.alpha + ver2.Y() * info.beta + ver3.Y() * info.gamma);
    }
    else if ( info.triangleId == 5 ) //left_2
    {
        ver1.X() = CurrentScene->_textures[info.textureId].width / 3;
        ver1.Y() = CurrentScene->_textures[info.textureId].height / 2;

        ver2.X() = 0;
        ver2.Y() = CurrentScene->_textures[info.textureId].height / 2;

        ver3.X() = 0;
        ver3.Y() = 0;

        info.j = round(ver1.X() * info.alpha + ver2.X() * info.beta + ver3.X() * info.gamma);
        info.i = round(ver1.Y() * info.alpha + ver2.Y() * info.beta + ver3.Y() * info.gamma);
    }
    else if ( info.triangleId == 6 ) //right_1
    {
        ver1.X() = CurrentScene->_textures[info.textureId].width / 3;
        ver1.Y() = CurrentScene->_textures[info.textureId].height / 2;

        ver2.X() = CurrentScene->_textures[info.textureId].width / 3;
        ver2.Y() = CurrentScene->_textures[info.textureId].height - 1;

        ver3.X() = 2 * CurrentScene->_textures[info.textureId].width / 3;
        ver3.Y() = CurrentScene->_textures[info.textureId].height / 2;

        info.j = round(ver1.X() * info.alpha + ver2.X() * info.beta + ver3.X() * info.gamma);
        info.i = round(ver1.Y() * info.alpha + ver2.Y() * info.beta + ver3.Y() * info.gamma);
    }
    else if ( info.triangleId == 7 ) //right_2
    {
        ver1.X() = 2 * CurrentScene->_textures[info.textureId].width / 3;
        ver1.Y() = CurrentScene->_textures[info.textureId].height;

        ver2.X() = CurrentScene->_textures[info.textureId].width / 3;
        ver2.Y() = CurrentScene->_textures[info.textureId].height;

        ver3.X() = CurrentScene->_textures[info.textureId].width / 3;
        ver3.Y() = CurrentScene->_textures[info.textureId].height / 2;

        info.j = round(ver1.X() * info.alpha + ver2.X() * info.beta + ver3.X() * info.gamma);
        info.i = round(ver1.Y() * info.alpha + ver2.Y() * info.beta + ver3.Y() * info.gamma);
    }
    else if ( info.triangleId == 8 ) //front_1 // ok
    {
        ver1.X() = 0;
        ver1.Y() = CurrentScene->_textures[info.textureId].height / 2;

        ver2.X() = CurrentScene->_textures[info.textureId].width / 3;
        ver2.Y() = CurrentScene->_textures[info.textureId].height / 2;

        ver3.X() = CurrentScene->_textures[info.textureId].width / 3;
        ver3.Y() = CurrentScene->_textures[info.textureId].height - 1;

        info.j = round(ver1.X() * info.gamma + ver2.X() * info.alpha + ver3.X() * info.beta);
        info.i = round(ver1.Y() * info.gamma + ver2.Y() * info.alpha + ver3.Y() * info.beta);
    }
    else if ( info.triangleId == 9 ) //front_2 //ok
    {
        ver1.X() = CurrentScene->_textures[info.textureId].width / 3;
        ver1.Y() = CurrentScene->_textures[info.textureId].height - 1;

        ver2.X() = 0;
        ver2.Y() = CurrentScene->_textures[info.textureId].height - 1;

        ver3.X() = 0;
        ver3.Y() = CurrentScene->_textures[info.textureId].height / 2;

        info.j = round(ver1.X() * info.beta + ver2.X() * info.gamma + ver3.X() * info.alpha);
        info.i = round(ver1.Y() * info.beta + ver2.Y() * info.gamma + ver3.Y() * info.alpha);
    }
    else if ( info.triangleId == 10 ) //back_1
    {
        ver1.X() = CurrentScene->_textures[info.textureId].width / 3;
        ver1.Y() = 0;

        ver2.X() = 2 * CurrentScene->_textures[info.textureId].width / 3;
        ver2.Y() = 0;

        ver3.X() = 2 * CurrentScene->_textures[info.textureId].width / 3;
        ver3.Y() = CurrentScene->_textures[info.textureId].height / 2;

        info.j = round(ver1.X() * info.alpha + ver2.X() * info.beta + ver3.X() * info.gamma);
        info.i = round(ver1.Y() * info.alpha + ver2.Y() * info.beta + ver3.Y() * info.gamma);
    }
    else if ( info.triangleId == 11 ) //back_2
    {
        ver1.X() = 2 * CurrentScene->_textures[info.textureId].width / 3;
        ver1.Y() = CurrentScene->_textures[info.textureId].height / 2;

        ver2.X() = CurrentScene->_textures[info.textureId].width / 3;
        ver2.Y() = CurrentScene->_textures[info.textureId].height / 2;

        ver3.X() = CurrentScene->_textures[info.textureId].width / 3;
        ver3.Y() = 0;

        info.j = round(ver1.X() * info.alpha + ver2.X() * info.beta + ver3.X() * info.gamma);
        info.i = round(ver1.Y() * info.alpha + ver2.Y() * info.beta + ver3.Y() * info.gamma);
    }
}

void Camera::findI_J( RayHitInfo& info, Sphere sphere ) const
{
    double  theta, fi, u, v;
    Vector3 temp(info.Position);

/*    for ( int j = sphere.transforms.size() - 1 ; j > -1 ; j-- )
    {
        if ( sphere.transforms[j] == 't' )
        {
            temp = translateBonus( &temp, translations[sphere.transformIDs[j] - 1], -1 );
        }
        else if ( sphere.transforms[j] == 'r' )
        {
            temp = rotationBonus( &temp, rotations[sphere.transformIDs[j] - 1] );
        }
    }
    for ( int j = 0 ; j < sphere.transforms.size() ; j++ )
    {
        if ( sphere.transforms[j] == 't' )
        {
            temp = translateBonus( &temp, translations[sphere.transformIDs[j] - 1], 1 );
        }
    }*/

    fi = -(atan2(temp.Z() - sphere.center.Z(), temp.X() - sphere.center.X())) + M_PI;
    if ( fi < 0 )
    { fi = 2 * M_PI + fi; }
    u  = fi / (2 * M_PI);
    info.j = round(u * CurrentScene->_textures[sphere.textureId].width);
    if ( info.j == CurrentScene->_textures[sphere.textureId].width )
    { info.j--; }

    theta = acos((temp.Y() - sphere.center.Y()) / sphere.radius);
    if ( theta < 0 )
    { theta += M_PI; }
    v     = (theta) / M_PI;
    info.i = round(v * CurrentScene->_textures[sphere.textureId].height);
    if ( info.i == CurrentScene->_textures[sphere.textureId].height )
    { info.i--; }
}