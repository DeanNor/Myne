
#pragma once

#include <cmath>

struct rad
{
public:
    double radian = 0;

    rad(double val);

    rad();

    operator double();

    // ----- ?
    bool operator> (rad compare);

    bool operator< (rad compare);

    bool operator>= (rad compare);

    bool operator<= (rad compare);

    bool operator== (rad compare);

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

rad operator rad(double amount);