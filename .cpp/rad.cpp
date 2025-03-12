#include "rad.hpp"

#include <iostream>

const double PI = 3.14159265358979323846;
double rad_constraint(double amount)
{
    if (amount > 0)
    {
        return fmod(amount, PI) - PI;
    }
    
    return fmod(amount, PI) + PI;
}

double constrain_rad(double amount)
{
    if (amount > PI || amount <= -PI)
    {
        return rad_constraint(amount);
    }

    return amount;
}

rad constrain_rad(rad amount)
{
    if (amount.radian > PI || amount.radian <= -PI)
    {
        return rad_constraint(amount.radian);
    }

    return amount.radian;
}

rad::rad(double val) : radian(constrain_rad(val))
{

}

rad::rad()
{
    radian = 0;
}

rad::operator double()
{
    return radian;
}

bool rad::operator> (rad compare)
{
    return constrain_rad(radian - compare.radian) > 0;
}

bool rad::operator< (rad compare)
{
    return constrain_rad(radian - compare.radian) < 0;
}

bool rad::operator>= (rad compare)
{
    return constrain_rad(radian - compare.radian) >= 0;
}

bool rad::operator<= (rad compare)
{
    return constrain_rad(radian - compare.radian) <= 0;
}

bool rad::operator== (rad compare)
{
    return radian == compare.radian;
}

rad rad::operator+ (rad amount)
{
    return amount.radian + radian;
}

rad rad::operator+ (double amount)
{
    return rad(amount + radian);
}

rad& rad::operator+= (rad amount)
{
    radian = radian + amount;

    radian = constrain_rad(radian);

    return *this;
}

rad& rad::operator+= (double amount)
{
    radian = radian + amount;

    radian = constrain_rad(radian);

    return *this;
}

rad rad::operator- (rad amount)
{
    return amount.radian - radian;
}

rad rad::operator- (double amount)
{
    return rad(radian - amount);
}

rad& rad::operator-= (rad amount)
{
    radian = radian - amount;

    radian = constrain_rad(radian);

    return *this;
}

rad& rad::operator-= (double amount)
{
    radian = radian - amount;

    radian = constrain_rad(radian);

    return *this;
}

rad rad::operator* (rad amount)
{
    return amount.radian * radian;
}

rad rad::operator* (double amount)
{
    return rad(radian * amount);
}

rad& rad::operator*= (rad amount)
{
    radian = radian * amount;

    radian = constrain_rad(radian);

    return *this;
}

rad& rad::operator*= (double amount)
{
    radian = radian * amount;

    radian = constrain_rad(radian);

    return *this;
}

rad rad::operator/ (rad amount)
{
    return amount.radian / radian;
}

rad rad::operator/ (double amount)
{
    return rad(radian / amount);
}

rad& rad::operator/= (rad amount)
{
    radian = radian / amount;

    radian = constrain_rad(radian);

    return *this;
}

rad& rad::operator/= (double amount)
{
    radian = radian / amount;

    radian = constrain_rad(radian);

    return *this;
}
