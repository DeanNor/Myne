
#pragma once

#include <box2d/math_functions.h>

#include "serial.hpp"

const double PI = 3.14159265358979323846;
const double TO_DEG = 180.0 / PI;

// Radian type, auto looping
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

    ARCHIVE_BASE(radian)
};

// Degree to radian
rad drad(double degree);
