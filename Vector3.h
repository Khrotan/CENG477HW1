#ifndef MATH_VECTOR3_H
#define MATH_VECTOR3_H

#include <iostream>

class Vector3
{
public:
    float _data[3];

    friend std::istream &operator>>(std::istream &stream, Vector3 &vertex)
    {
        return stream >> vertex._data[0] >> vertex._data[1] >> vertex._data[2];
    }

    Vector3 normalize();

    double dotProduct( const Vector3& rhs ) const;

    double getLength();

    Vector3 crossProduct( const Vector3& rhs );

    Vector3 operator+( const Vector3& rhs ) const;

    Vector3 operator-( const Vector3& rhs ) const;

    Vector3 operator*( int scalar ) const;

    Vector3 operator*( float scalar ) const;

    Vector3 operator*( double scalar ) const;

    Vector3 operator/( float scalar ) const;

    Vector3 operator/( double scalar ) const;
};

#endif //MATH_VECTOR3_H
