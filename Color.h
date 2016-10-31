#ifndef RAYTRACER_COLOR_H
#define RAYTRACER_COLOR_H

#include <iostream>

class Color {
public:
    float _channels[3];

    Color();
    Color(float r, float g, float b);

    float R() const
    {
        return _channels[0];
    }

    float G() const
    {
        return _channels[1];
    }

    float B() const
    {
        return _channels[2];
    }

    Color operator*(Color c) const
    {
        return Color(this->_channels[0] * c._channels[0],
                     this->_channels[1] * c._channels[1],
                     this->_channels[2] * c._channels[2]);
    }

    Color operator*(float c) const
    {
        return Color(_channels[0] * c,
                     _channels[1] * c,
                     _channels[2] * c);
    }

    Color operator/(float denom) const
    {
        return (*this) * (1 / denom);
    }

    friend std::istream& operator>>(std::istream& stream, Color& color);
};


inline Color::Color()
{
    _channels[0] = _channels[1] = _channels[2] = 0;
}

inline Color::Color(float r, float g, float b) {
    _channels[0] = r;
    _channels[1] = g;
    _channels[2] = b;
}

inline std::istream &operator>>(std::istream &stream, Color &color) {
    return stream >> color._channels[0] >> color._channels[1] >> color._channels[2];
}


#endif //RAYTRACER_COLOR_H
