#ifndef RAYTRACER_SCENE_H
#define RAYTRACER_SCENE_H

#include <vector>
#include <cstdlib>
#include <fstream>
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
    Color _backgroundColour;
    Color _ambientLight;

    const std::vector<Camera>& Cameras() const {
        return _cameras;
    }

    //////////////////////////////
    // READ SCENE FUNCTIONS HERE !
    //////////////////////////////
};

void ReadScene(int argc, char** argv);

extern Scene* CurrentScene;

#endif //RAYTRACER_SCENE_H
