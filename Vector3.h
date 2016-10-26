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

    double dotProduct( const Vector3& rhs ) {
        return this->_data[0] * rhs._data[0] + this->_data[1] * rhs._data[1] + this->_data[2] * rhs._data[2];
    }

    Vector3 crossProduct( const Vector3& rhs ) {
        Vector3 returnVector;
        returnVector._data[0] = this->_data[1] * rhs._data[2] - this->_data[2] * rhs._data[1];
        returnVector._data[1] = this->_data[2] * rhs._data[0] - this->_data[0] * rhs._data[2];
        returnVector._data[2] = this->_data[0] * rhs._data[1] - this->_data[1] * rhs._data[0];

        return returnVector;
    }

    Vector3 operator+( const Vector3& rhs ) const {
        Vector3 returnVector;
        returnVector._data[0] = this->_data[0] + rhs._data[0];
        returnVector._data[1] = this->_data[1] + rhs._data[1];
        returnVector._data[2] = this->_data[2] + rhs._data[2];

        return returnVector;
    }

    Vector3 operator-( const Vector3& rhs ) const {
        Vector3 returnVector;
        returnVector._data[0] = this->_data[0] - rhs._data[0];
        returnVector._data[1] = this->_data[1] - rhs._data[1];
        returnVector._data[2] = this->_data[2] - rhs._data[2];

        return returnVector;
    }

    Vector3 operator*( int scalar ) const {
        Vector3 returnVector;
        returnVector._data[0] = this->_data[0] * scalar;
        returnVector._data[1] = this->_data[1] * scalar;
        returnVector._data[2] = this->_data[2] * scalar;

        return returnVector;
    }

    Vector3 operator*( float scalar ) const {
        Vector3 returnVector;
        returnVector._data[0] = this->_data[0] * scalar;
        returnVector._data[1] = this->_data[1] * scalar;
        returnVector._data[2] = this->_data[2] * scalar;

        return returnVector;
    }

    Vector3 operator*( double scalar ) const {
        Vector3 returnVector;
        returnVector._data[0] = this->_data[0] * scalar;
        returnVector._data[1] = this->_data[1] * scalar;
        returnVector._data[2] = this->_data[2] * scalar;

        return returnVector;
    }
};

#endif //MATH_VECTOR3_H