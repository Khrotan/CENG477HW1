//
// Created by Arda on 26.10.2016.
//

#include <math.h>
#include "Vector3.h"


double Vector3::dotProduct( const Vector3& rhs ) const
{
    return ( this->_data[0] * rhs._data[0] + this->_data[1] * rhs._data[1] + this->_data[2] * rhs._data[2] );
}

Vector3 Vector3::crossProduct( const Vector3& rhs )
{
    Vector3 returnVector;
    returnVector._data[0] = ( this->_data[1] * rhs._data[2] - this->_data[2] * rhs._data[1] );
    returnVector._data[1] = ( this->_data[2] * rhs._data[0] - this->_data[0] * rhs._data[2] );
    returnVector._data[2] = ( this->_data[0] * rhs._data[1] - this->_data[1] * rhs._data[0] );

    return returnVector;
}

Vector3 Vector3::operator+( const Vector3& rhs ) const
{
    Vector3 returnVector;
    returnVector._data[0] = this->_data[0] + rhs._data[0];
    returnVector._data[1] = this->_data[1] + rhs._data[1];
    returnVector._data[2] = this->_data[2] + rhs._data[2];

    return returnVector;
}

Vector3 Vector3::operator-( const Vector3& rhs ) const
{
    Vector3 returnVector;
    returnVector._data[0] = this->_data[0] - rhs._data[0];
    returnVector._data[1] = this->_data[1] - rhs._data[1];
    returnVector._data[2] = this->_data[2] - rhs._data[2];

    return returnVector;
}

Vector3 Vector3::operator*( int scalar ) const
{
    Vector3 returnVector;
    returnVector._data[0] = this->_data[0] * scalar;
    returnVector._data[1] = this->_data[1] * scalar;
    returnVector._data[2] = this->_data[2] * scalar;

    return returnVector;
}

Vector3 Vector3::operator*( float scalar ) const
{
    Vector3 returnVector;
    returnVector._data[0] = this->_data[0] * scalar;
    returnVector._data[1] = this->_data[1] * scalar;
    returnVector._data[2] = this->_data[2] * scalar;

    return returnVector;
}

Vector3 Vector3::operator*( double scalar ) const
{
    Vector3 returnVector;
    returnVector._data[0] = this->_data[0] * scalar;
    returnVector._data[1] = this->_data[1] * scalar;
    returnVector._data[2] = this->_data[2] * scalar;

    return returnVector;
}

Color Vector3::operator*( Color c ) const
{
    Color returnColor;
    returnColor._channels[0] = this->_data[0] * c._channels[0];
    returnColor._channels[1] = this->_data[1] * c._channels[1];
    returnColor._channels[2] = this->_data[2] * c._channels[2];

    return returnColor;
}


Vector3 Vector3::operator/( float scalar ) const
{
    Vector3 returnVector;
    returnVector._data[0] = this->_data[0] / scalar;
    returnVector._data[1] = this->_data[1] / scalar;
    returnVector._data[2] = this->_data[2] / scalar;

    return returnVector;
}

Vector3 Vector3::operator/( double scalar ) const
{
    Vector3 returnVector;
    returnVector._data[0] = this->_data[0] / scalar;
    returnVector._data[1] = this->_data[1] / scalar;
    returnVector._data[2] = this->_data[2] / scalar;

    return returnVector;
}

Vector3 Vector3::normalize()
{
    double length = this->getLength();
    this->_data[0] /= length;
    this->_data[1] /= length;
    this->_data[2] /= length;

    return *this;
}

double Vector3::getLength()
{
    return sqrt( this->_data[0] * this->_data[0] + this->_data[1] * this->_data[1] + this->_data[2] * this->_data[2] );
}
