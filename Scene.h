#ifndef RAYTRACER_SCENE_H
#define RAYTRACER_SCENE_H

#include <vector>
#include <cstdlib>
#include "Camera.h"
#include "Sphere.h"
#include "Mesh.h"
#include "Color.h"
#include "PointLight.h"
#include "Vertex.h"

using namespace std;

class Scene {
public:
    std::vector<Vertex> _vertices;

    std::vector<Camera> _cameras;
    std::vector<Sphere> _spheres;
    std::vector<Mesh>   _meshes;

    std::vector<Material> _materials;

    std::vector<PointLight> _lights;

    int _rayReflect;
    Color _background;
    Color _ambient;

    const std::vector<Camera>& Cameras() const {
        return _cameras;
    }

    //////////////////////////////
    // READ SCENE FUNCTIONS HERE !
    //////////////////////////////
};

Scene* CurrentScene;

void ReadScene(int argc, char** argv)
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

    std::ifstream infile;
    infile.open( argv[2] );

    if ( !infile ) {
        std::cout << "Camera can't open." << std::endl;
        return;
    }

    int count;

    infile >> count;

    std::cout << "Cam Count: " << count << std::endl;

    for ( int i = 0 ; i < count ; i++ ) {
        Camera dummyCamera;
        infile >> dummyString;
        infile >> dummyString;

        //infile >> dummyCamera._position._data[0] >> dummyCamera._position._data[1] >> dummyCamera._position._data[2];
        infile >> dummyCamera._position;
        infile >> dummyCamera._gaze;
        infile >> dummyCamera._space.Up;
        infile >> dummyCamera._imagePlane.Left >> dummyCamera._imagePlane.Right >> dummyCamera._imagePlane.Bottom >> dummyCamera._imagePlane.Top >> dummyCamera._imagePlane.Distance >> dummyCamera._imagePlane.Width >> dummyCamera._imagePlane.Height;
        infile >> dummyCamera._outputFile;

        dummyCamera._space.Forward._data[0] = - dummyCamera._gaze._data[0];
        dummyCamera._space.Forward._data[1] = - dummyCamera._gaze._data[1];
        dummyCamera._space.Forward._data[2] = - dummyCamera._gaze._data[2];

        //TODO: u : v x w
        dummyCamera._space.Left = dummyCamera._space.Up.crossProduct( dummyCamera._space.Forward );

        scene->_cameras.push_back( dummyCamera );
    }

    infile.close();


    //SCENE
    infile.open( argv[1] );

    if ( !infile ) {
        std::cout << "Scene can't open." << std::endl;
        return;
    }

    infile >> scene->_rayReflect;
    infile >> scene->_background;
    infile >> scene->_ambient;

    infile >> count;
    for ( int i = 0 ; i < count; i++ ) {
        PointLight dummyPointLight;
        infile >> dummyPointLight.position;
        infile >> dummyPointLight.intensity;
        scene->_lights.push_back(dummyPointLight);
    }

    infile >> count;
    for ( int i = 0 ; i < count ; i++ ) {
        infile >> dummyString >> dummyString;

        Material dummyMaterial;
        infile >> dummyMaterial.ambient;
        infile >> dummyMaterial.diffuse;
        infile >> dummyMaterial.specular >> dummyMaterial.specExp;
        infile >> dummyMaterial.reflectance;

        scene->_materials.push_back(dummyMaterial);
    }

    infile >> count;

    //#Vertex Data
    infile >> dummyString >> dummyString;

    for ( int i = 0 ; i < count ; i++ ) {
        Vertex dummyVertex;
        infile >> dummyVertex;

        scene->_vertices.push_back( dummyVertex );
    }

    infile >> count;
    for ( int i = 0 ; i < count ; i++ ) {
        string dummyString1, dummyString2;
        infile >> dummyString1 >> dummyString2;

        if ( dummyString1.find("Mesh") != string::npos ) {
            Mesh dummyMesh;
            infile >> dummyMesh.triangleCount;
            infile >> dummyMesh.materialId;

            int vertId;
            infile >> vertId;
            dummyMesh.pos1 = scene->_vertices[vertId]._position;

            infile >> vertId;
            dummyMesh.pos2 = scene->_vertices[vertId]._position;

            infile >> vertId;
            dummyMesh.pos3 = scene->_vertices[vertId]._position;

            scene->_meshes.push_back( dummyMesh );
        } else {
            Sphere dummySphere;

            infile >> dummySphere.materialId;
            infile >> dummySphere.radius;

            int vertId;
            infile >> vertId;
            dummySphere.center = scene->_vertices[vertId]._position;

            scene->_spheres.push_back( dummySphere );
        }
    }
    CurrentScene = scene;
}

#endif //RAYTRACER_SCENE_H
