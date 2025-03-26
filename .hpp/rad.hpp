
#pragma once

#include <cmath>

#include <box2d/math_functions.h>

struct rad
{
public:
    double radian = 0;

    double deg();

    rad(const double& val);

    rad(const b2Rot& val);

    rad();

    operator double();

    operator b2Rot();

    // ----- ?
    bool operator> (rad compare);

    bool operator< (rad compare);

    bool operator>= (rad compare);

    bool operator<= (rad compare);

    bool operator== (rad compare);

    bool operator!= (rad compare);

    // ----- +
    rad operator+ (rad amount);

    rad operator+ (double amount);

    // ----- +=
    rad& operator+= (rad amount);

    rad& operator+= (double amount);

    // ----- -
    rad operator- (rad amount);

    rad operator- (double amount);

    // ----- -=
    rad& operator-= (rad amount);

    rad& operator-= (double amount);

    // ----- *
    rad operator* (rad amount);

    rad operator* (double amount);
    
    // ----- *=
    rad& operator*= (rad amount);
    
    rad& operator*= (double amount);

    // ----- /
    rad operator/ (rad amount);

    rad operator/ (double amount);

    // ----- /=
    rad& operator/= (rad amount);

    rad& operator/= (double amount);
};
