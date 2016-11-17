//
// Created by Arda on 26.10.2016.
//

#include "Scene.h"


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
        infile >> dummyRotation.alpha >> dummyRotation.u_x >> dummyRotation.u_y >> dummyRotation.u_z;

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

            for ( int j = 0 ; j < count ; ++j )
            {
                int translationIndex;
                infile >> dummyString >> translationIndex;

                if ( dummyString == "s" ) {
                    Scaling dummyScaling;
                    infile >> dummyScaling.s_x >> dummyScaling.s_y >> dummyScaling.s_z;

                    scene->_scalings.push_back( dummyScaling );
                } else if ( dummyString == "r" ) {
                    Rotation dummyRotation;
                    infile >> dummyRotation.alpha >> dummyRotation.u_x >> dummyRotation.u_y >> dummyRotation.u_z;

                    scene->_rotations.push_back( dummyRotation );
                } else {
                    Translation dummyTranslation;
                    infile >> dummyTranslation.t_x >> dummyTranslation.t_y >> dummyTranslation.t_z;

                    scene->_translations.push_back( dummyTranslation );
                }
            }

            scene->_spheres.push_back( dummySphere );
        }
        else {

        }
    }

/*    //#Vertex Data
    infile >> dummyString >> dummyString;

    for ( int i = 0 ; i < count ; i++ )
    {
        Vertex dummyVertex;
        infile >> dummyVertex;

        scene->_vertices.push_back( dummyVertex );
    }

    infile >> count;
    for ( int i = 0 ; i < count ; i++ )
    {
        string dummyString1, dummyString2;
        infile >> dummyString1 >> dummyString2;

        if ( dummyString1.find( "Mesh" ) != string::npos )
        {
            Mesh dummyMesh;
            infile >> dummyMesh.triangleCount;
            infile >> dummyMesh.materialId;

            int vertId;
            for ( int m = 0 ; m < dummyMesh.triangleCount ; m++ )
            {
                Triangle dummyTriangle;

                infile >> vertId;
                dummyTriangle.Vid1 = scene->_vertices[vertId]._position;

                infile >> vertId;
                dummyTriangle.Vid2 = scene->_vertices[vertId]._position;

                infile >> vertId;
                dummyTriangle.Vid3 = scene->_vertices[vertId]._position;

                dummyTriangle.normal = ( dummyTriangle.Vid2 - dummyTriangle.Vid1 ).crossProduct(
                        dummyTriangle.Vid3 - dummyTriangle.Vid1 ).normalize();

                dummyMesh.triangles.push_back( dummyTriangle );
            }

            scene->_meshes.push_back( dummyMesh );
        }
        else
        {
            Sphere dummySphere;

            infile >> dummySphere.materialId;
            infile >> dummySphere.radius;

            int vertId;
            infile >> vertId;
            dummySphere.center = scene->_vertices[vertId]._position;

            scene->_spheres.push_back( dummySphere );
        }
    }*/
    CurrentScene = scene;
}

Scene* CurrentScene;
