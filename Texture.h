//
// Created by Arda on 16.11.2016.
//

#ifndef EDIT_TEXTURE_H
#define EDIT_TEXTURE_H

#include <string>
extern "C" {
#include "jpeg_reader/aoa_jpeg.h"
}

class Texture {
public:
    std::string fileName;
    int height;
    int width;
    UCOLOR** image;
};

#endif //EDIT_TEXTURE_H
