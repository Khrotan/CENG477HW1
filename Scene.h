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
#include "Texture.h"
#include "Translation.h"
#include "Scaling.h"
#include "Rotation.h"
#include "Cube.h"

using namespace std;

class Scene {
public:
    std::vector<Vertex> _vertices;

    std::vector<Camera> _cameras;
    std::vector<Sphere> _spheres;
    std::vector<Mesh>   _meshes;
    std::vector<Cube> _cubes;

    std::vector<Material> _materials;

    std::vector<Texture> _textures;

    std::vector<Translation> _translations;
    std::vector<Scaling> _scalings;
    std::vector<Rotation> _rotations;

    std::vector<PointLight> _lights;

    int _rayReflect;
    Color _backgroundColour;
    Color _ambientLight;

    const std::vector<Camera>& Cameras() const {
        return _cameras;
    }

    void applyTransformations();

    void rotatePoint( Vector3& point, Rotation rotation );

    //////////////////////////////
    // READ SCENE FUNCTIONS HERE !
    //////////////////////////////
};

void ReadScene(int argc, char** argv);

extern Scene* CurrentScene;

#endif //RAYTRACER_SCENE_H
