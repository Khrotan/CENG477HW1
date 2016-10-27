#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H

#include "Color.h"
#include "Vector3.h"

class Scene;

class Material {
    //////////////////
    /// PUT YOUR VARIABLES, HELPER FUNCTIONS HERE !
    //////////////////
public:
    Color ambientCoefficient;
    Color diffuseCoefficient;
    Color specularCoefficient;
    int specExp;
    Color reflectance;

    /*
     * Implement a Calculate function that does Diffuse, Specular and Ambient, Reflective shading
     */

    friend std::istream& operator>>(std::istream& stream, Material& mat);
};

using MaterialId = int;

#endif //RAYTRACER_MATERIAL_H
