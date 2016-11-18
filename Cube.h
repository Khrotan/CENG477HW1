//
// Created by Arda on 16.11.2016.
//

#ifndef EDIT_CUBE_H
#define EDIT_CUBE_H


#include <Vector>
#include "Triangle.h"
#include "Translation.h"
#include "Rotation.h"
#include "Scaling.h"

class Cube
{
public:
    int materialId;
    int textureId;

    std::vector<Translation> translations;
    std::vector<Rotation> rotations;
    std::vector<Scaling> scalings;

    Cube();

    Vector3 solArkaUst;
    Vector3 solOnUst;

    Vector3 sagOnUst;
    Vector3 sagArkaUst;

    Vector3 solArkaAlt;
    Vector3 solOnAlt;

    Vector3 sagOnAlt;
    Vector3 sagArkaAlt;

    //6 faces
    //up bottom
    //left right
    //front back
    Triangle up_1;
    Triangle up_2;
    Triangle bottom_1;
    Triangle bottom_2;

    Triangle left_1;
    Triangle left_2;
    Triangle right_1;
    Triangle right_2;

    Triangle front_1;
    Triangle front_2;
    Triangle back_1;
    Triangle back_2;

    std::vector<Triangle> triangles;

    void applyScaling( Scaling scaling );

    void applyTranslation( Translation translation );
};

#endif //EDIT_CUBE_H
