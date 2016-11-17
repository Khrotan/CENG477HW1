#ifndef RAYTRACER_COLOR_H
#define RAYTRACER_COLOR_H

#include <iostream>

class Color {
public:
    double _channels[3];

    Color();
    Color(double r, double g, double b);

    double R() const
    {
        return _channels[0];
    }

    double G() const
    {
        return _channels[1];
    }

    double B() const
    {
        return _channels[2];
    }

    Color operator*(Color c) const
    {
        return Color(this->_channels[0] * c._channels[0],
                     this->_channels[1] * c._channels[1],
                     this->_channels[2] * c._channels[2]);
    }

    Color operator+(Color c) const
    {
        return Color(this->_channels[0] + c._channels[0],
                     this->_channels[1] + c._channels[1],
                     this->_channels[2] + c._channels[2]);
    }

    Color operator*(double c) const
    {
        return Color(_channels[0] * c,
                     _channels[1] * c,
                     _channels[2] * c);
    }

    Color operator/(double denom) const
    {
        return (*this) * (1 / denom);
    }

    friend std::istream& operator>>(std::istream& stream, Color& color);

    bool isShiny();
};


inline Color::Color()
{
    _channels[0] = _channels[1] = _channels[2] = 0;
}

inline Color::Color(double r, double g, double b) {
    _channels[0] = std::max(0.0, std::min(255.0, r));
    _channels[1] = std::max(0.0, std::min(255.0, g));
    _channels[2] = std::max(0.0, std::min(255.0, b));
}

inline std::istream &operator>>(std::istream &stream, Color &color) {
    return stream >> color._channels[0] >> color._channels[1] >> color._channels[2];
}

inline bool Color::isShiny()
{
    return (this->_channels[0] != 0 && this->_channels[1] != 0 && this->_channels[2] != 0);
}


#endif //RAYTRACER_COLOR_H
