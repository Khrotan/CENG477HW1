//
// Created by Arda on 16.11.2016.
//

#ifndef EDIT_CUBE_H
#define EDIT_CUBE_H


#include "Triangle.h"

class Cube
{
    //6 faces
    //up bottom
    //left right
    //front back
    Triangle up_1, up_2;
    Triangle bottom_1, bottom2;

    Triangle left_1, left_2;
    Triangle right_1, right_2;

    Triangle front_1, front_2;
    Triangle back_1, back_2;


};

#endif //EDIT_CUBE_H
