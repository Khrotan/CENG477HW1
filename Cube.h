//
// Created by Arda on 16.11.2016.
//

#ifndef EDIT_CUBE_H
#define EDIT_CUBE_H


#include "Triangle.h"

class Cube
{
    Vector3 solArkaUst(-0.5, 0.5, -0.5);
    Vector3 solOnUst(-0.5, 0.5, 0.5);

    Vector3 sagOnUst(0.5, 0.5, 0.5);
    Vector3 sagArkaUst(0.5, 0.5, -0.5);

    Vector3 solArkaAlt(-0.5, -0.5, -0.5);
    Vector3 solOnAlt(-0.5, -0.5, 0.5);

    Vector3 sagOnAlt(0.5, -0.5, 0.5);
    Vector3 sagArkaAlt(0.5, -0.5, -0.5);

    //6 faces
    //up bottom
    //left right
    //front back
    Triangle up_1(solOnUst, sagArkaUSt, solArkaUst), up_2(solOnUst, sagArkaUst, sagOnUst);
    Triangle bottom_1(solArkaAlt, sagArkaAlt, solOnAlt), bottom2(sagArkaAlt, sagOnAlt, solOnAlt);

    Triangle left_1(solArkaUst, solOnAlt, solOnUst), left_2(solArkaUst, solArkaAlt, solOnAlt);
    Triangle right_1(sagArkaUst, sagArkaAlt, sagOnUst), right_2(sagArkaAlt, sagOnAlt, sagOnUst);

    Triangle front_1(sagOnUst, sagOnAlt, solOnAlt), front_2(solOnUst, sagOnUst, solOnAlt);
    Triangle back_1(sagArkaUst, sagArkaAlt, solArkaAlt), back_2(solArkaUst, sagArkaUst, solArkaAlt);


};

#endif //EDIT_CUBE_H
