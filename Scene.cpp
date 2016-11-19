//
// Created by Arda on 26.10.2016.
//
#include "jpeg_reader/aoa_jpeg.h"
#include "Scene.h"


#define M_PI 3.14159265358979323846

void ReadScene( int argc, char** argv )
{
    /*
     * Reads the scene and cameras from arguments
     * Then sets the CurrentScene pointer
     */
    string dummyString;
    Scene* scene = new Scene;
    // dummy objects to ease indexing
    scene->_vertices.push_back( Vertex() );
    scene->_materials.push_back( Material() );
    scene->_translations.push_back( Translation() );
    scene->_scalings.push_back( Scaling() );
    scene->_rotations.push_back( Rotation() );
    scene->_textures.push_back( Texture() );

    std::ifstream infile;
    infile.open( argv[2] );

    if ( !infile )
    {
        std::cout << "Camera can't open." << std::endl;
        return;
    }

    int count;

    infile >> count;

    for ( int i = 0 ; i < count ; i++ )
    {
        Camera dummyCamera;
        infile >> dummyString;
        infile >> dummyString;

        infile >> dummyCamera._position;
        infile >> dummyCamera._gaze;
        infile >> dummyCamera._space.Up;
        infile >> dummyCamera._imagePlane.Left >> dummyCamera._imagePlane.Right >> dummyCamera._imagePlane.Bottom
               >> dummyCamera._imagePlane.Top >> dummyCamera._imagePlane.Distance >> dummyCamera._imagePlane.Width
               >> dummyCamera._imagePlane.Height;
        infile >> dummyCamera._outputFile;

        dummyCamera._space.Forward._data[0] = -dummyCamera._gaze._data[0];
        dummyCamera._space.Forward._data[1] = -dummyCamera._gaze._data[1];
        dummyCamera._space.Forward._data[2] = -dummyCamera._gaze._data[2];

        //TODO: u : v x w
        dummyCamera._space.Left = dummyCamera._space.Up.crossProduct( dummyCamera._space.Forward );

        scene->_cameras.push_back( dummyCamera );
    }

    infile.close();


    //SCENE
    infile.open( argv[1] );

    if ( !infile )
    {
        std::cout << "Scene can't open." << std::endl;
        return;
    }

    infile >> scene->_rayReflect;
    infile >> scene->_backgroundColour;
    infile >> scene->_ambientLight;

    infile >> count;
    for ( int i = 0 ; i < count ; i++ )
    {
        PointLight dummyPointLight;
        infile >> dummyPointLight.position;
        infile >> dummyPointLight.intensity;
        scene->_lights.push_back( dummyPointLight );
    }

    infile >> count;
    for ( int i = 0 ; i < count ; i++ )
    {
        infile >> dummyString >> dummyString;

        Material dummyMaterial;
        infile >> dummyMaterial.ambientCoefficient;
        infile >> dummyMaterial.diffuseCoefficient;
        infile >> dummyMaterial.specularCoefficient >> dummyMaterial.specExp;
        infile >> dummyMaterial.reflectance;

        scene->_materials.push_back( dummyMaterial );
    }

    //#Textures
    infile >> dummyString;

    //texture
    infile >> count;

    for ( int i = 0 ; i < count ; i++ )
    {
        Texture dummyTexture;
        infile >> dummyTexture.fileName;

        read_jpeg_header( dummyTexture.fileName.c_str(), &dummyTexture.width, &dummyTexture.height );

        dummyTexture.image = (UCOLOR**) malloc( sizeof( UCOLOR** ) * ( dummyTexture.height ) );

        for ( int j = 0 ; j < dummyTexture.height ; j++ )
        {
            dummyTexture.image[j] = (UCOLOR*) malloc( sizeof( UCOLOR* ) * dummyTexture.width );
        }

        read_jpeg( dummyTexture.fileName.c_str(), dummyTexture.image, &dummyTexture.width, &dummyTexture.height );

        scene->_textures.push_back( dummyTexture );
    }

    //#Translation
    infile >> dummyString;

    //translation
    infile >> count;

    for ( int i = 0 ; i < count ; ++i )
    {
        Translation dummyTranslation;
        infile >> dummyTranslation.t_x >> dummyTranslation.t_y >> dummyTranslation.t_z;

        scene->_translations.push_back( dummyTranslation );
    }

    //#Scaling
    infile >> dummyString;

    //scaling
    infile >> count;

    for ( int i = 0 ; i < count ; ++i )
    {
        Scaling dummyScaling;
        infile >> dummyScaling.s_x >> dummyScaling.s_y >> dummyScaling.s_z;

        scene->_scalings.push_back( dummyScaling );
    }

    //#Rotation
    infile >> dummyString;

    //rotation
    infile >> count;

    for ( int i = 0 ; i < count ; ++i )
    {
        Rotation dummyRotation;
        infile >> dummyRotation.alpha >> dummyRotation.point._data[0] >> dummyRotation.point._data[1]
               >> dummyRotation.point._data[2];

        dummyRotation.point.normalize();

        scene->_rotations.push_back( dummyRotation );
    }

    //instance
    infile >> count;

    int transformationCount;

    for ( int i = 0 ; i < count ; ++i )
    {
        infile >> dummyString;

        if ( dummyString == "#SphereInstance" )
        {
            Sphere dummySphere;

            infile >> dummySphere.materialId;
            infile >> dummySphere.textureId;

            infile >> transformationCount;

            for ( int j = 0 ; j < transformationCount ; ++j )
            {
                int transformationIndex;
                infile >> dummyString >> transformationIndex;

                if ( dummyString == "s" )
                {
                    dummySphere.scalings.push_back( scene->_scalings[transformationIndex] );
                }
                else if ( dummyString == "r" )
                {
                    dummySphere.rotations.push_back( scene->_rotations[transformationIndex] );
                }
                else
                {
                    dummySphere.translations.push_back( scene->_translations[transformationIndex] );
                }
            }

            scene->_spheres.push_back( dummySphere );
        }
        else
        {
            Cube dummyCube;

            infile >> dummyCube.materialId;
            infile >> dummyCube.textureId;

            infile >> transformationCount;

            for ( int j = 0 ; j < transformationCount ; ++j )
            {
                int transformationIndex;
                infile >> dummyString >> transformationIndex;

                if ( dummyString == "s" )
                {
                    dummyCube.scalings.push_back( scene->_scalings[transformationIndex] );
                }
                else if ( dummyString == "r" )
                {
                    dummyCube.rotations.push_back( scene->_rotations[transformationIndex] );
                }
                else
                {
                    dummyCube.translations.push_back( scene->_translations[transformationIndex] );
                }
            }

            scene->_cubes.push_back( dummyCube );
        }
    }

    CurrentScene = scene;

    CurrentScene->applyTransformations();
}

Scene* CurrentScene;

void Scene::rotatePoint( Vector3& point, Rotation rotation )
{
    //rotation.point.normalize();

    double a, b, c, d, x, y, z;
    a = rotation.point.X();
    b = rotation.point.Y();
    c = rotation.point.Z();
    d = sqrt( ( b * b ) + ( c * c ) );

    if ( d != 0 )
    {
        y = ( (point.Y() * c ) / d ) - ( ( point.Z() * b ) / d );
        z = ( (point.Y() * b ) / d ) + ( ( point.Z() * c ) / d );
        point._data[1] = y;
        point._data[2] = z;
    }

    x = ( point.X() * d ) - ( point.Z() * a );
    z = ( point.X() * a ) + ( point.Z() * d );
    point._data[0] = x;
    point._data[2] = z;
    x = ( point.X() * cos( ( rotation.alpha * M_PI ) / 180.0 ) ) -
        ( point.Y() * sin( ( rotation.alpha * M_PI ) / 180.0 ) );
    y = ( point.X() * sin( ( rotation.alpha * M_PI ) / 180.0 ) ) +
        ( point.Y() * cos( ( rotation.alpha * M_PI ) / 180.0 ) );
    point._data[0] = x;
    point._data[1] = y;

    x = ( point.X() * d ) + ( point.Z() * a );
    z = ( point.Z() * d ) - ( point.X() * a );
    point._data[0] = x;
    point._data[2] = z;

    if ( d != 0 )
    {
        y = ( ( point.Y() * c ) / d ) + ( ( point.Z() * b ) / d );
        z = ( ( point.Z() * c ) / d ) - ( ( point.Y() * b ) / d );
        point._data[1] = y;
        point._data[2] = z;
    }
}


void Scene::applyTransformations()
{
    for ( auto& cube : this->_cubes )
    {
        for ( auto& scaling : cube.scalings )
        {
            cube.applyScaling( scaling );
        }

        for ( auto& translation : cube.translations )
        {
            cube.applyTranslation( translation );
        }

/*        for ( auto& rotation : cube.rotations )
        {
            for ( auto& triangle : cube.triangles )
            {
                this->rotatePoint( triangle.Vid1, rotation );
                this->rotatePoint( triangle.Vid2, rotation );
                this->rotatePoint( triangle.Vid3, rotation );
                triangle.computeNormal();
            }
        }*/
    }

    for ( auto& sphere : this->_spheres )
    {
        for ( auto& scaling : sphere.scalings )
        {
            sphere.applyScaling( scaling );
        }

        for ( auto& translation : sphere.translations )
        {
            sphere.applyTranlation( translation );
        }

        for ( auto& rotation : sphere.rotations )
        {
            this->rotatePoint( sphere.center, rotation );
        }
    }
}