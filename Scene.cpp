//
// Created by Arda on 26.10.2016.
//

#include "Scene.h"
#include "Cube.h"

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

    cout << "arda" << endl;
}

Scene* CurrentScene;

void Scene::rotatePoint( Vector3& point, Rotation rotation )
{
    rotation.point.normalize();

    double a, b, c, d, x, y, z;
    a = rotation.point._data[0];
    b = rotation.point._data[1];
    c = rotation.point._data[2];
    d = sqrt( ( b * b ) + ( c * c ) );

    if ( d != 0 )
    {
        y = ( ( point._data[1] * c ) / d ) - ( ( point._data[2] * b ) / d );
        z = ( ( point._data[1] * b ) / d ) + ( ( point._data[2] * c ) / d );
        point._data[1] = y;
        point._data[2] = z;
    }

    x = ( point._data[0] * d ) - ( point._data[2] * a );
    z = ( point._data[0] * a ) + ( point._data[2] * d );
    point._data[0] = x;
    point._data[2] = z;
    x = ( point._data[0] * cos( ( rotation.alpha * M_PI ) / 180.0 ) ) -
        ( point._data[1] * sin( ( rotation.alpha * M_PI ) / 180.0 ) );
    y = ( point._data[0] * sin( ( rotation.alpha * M_PI ) / 180.0 ) ) +
        ( point._data[1] * cos( ( rotation.alpha * M_PI ) / 180.0 ) );
    point._data[0] = x;
    point._data[1] = y;

    x = ( point._data[0] * d ) + ( point._data[2] * a );
    z = ( point._data[2] * d ) - ( point._data[0] * a );
    point._data[0] = x;
    point._data[2] = z;

    if ( d != 0 )
    {
        y = ( ( point._data[1] * c ) / d ) + ( ( point._data[2] * b ) / d );
        z = ( ( point._data[2] * c ) / d ) - ( ( point._data[1] * b ) / d );
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

        for ( auto& rotation : cube.rotations )
        {
            for ( auto& triangle : cube.triangles )
            {
                this->rotatePoint( triangle.Vid1, rotation );
                this->rotatePoint( triangle.Vid2, rotation );
                this->rotatePoint( triangle.Vid3, rotation );
                triangle.computeNormal();
            }
        }
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
            ;
        }
    }
}