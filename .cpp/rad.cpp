
#include "rad.hpp"

#define PI 3.14159
double constrain_rad(double amount)
{
    if (amount > PI || amount <= -PI)
    {
        return PI - fmod(amount, PI);
    }

    return amount;
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
    return radian > compare.radian;
}

bool rad::operator< (rad compare)
{
    return radian < compare.radian;
}

bool rad::operator>= (rad compare)
{
    return radian >= compare.radian;
}

bool rad::operator<= (rad compare)
{
    return radian <= compare.radian;
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
    return rad(amount.radian + radian);
}

rad& rad::operator+= (rad amount)
{
    radian = radian + amount;

    radian = constrain_rad(radian);

    return *this;
}

rad rad::operator- (rad amount)
{
    return amount.radian - radian;
}

rad& rad::operator-= (rad amount)
{
    radian = radian - amount;

    radian = constrain_rad(radian);

    return *this;
}

rad rad::operator* (rad amount)
{
    return amount.radian * radian;
}

rad& rad::operator*= (rad amount)
{
    radian = radian * amount;

    radian = constrain_rad(radian);

    return *this;
}

rad rad::operator/ (rad amount)
{
    return amount.radian / radian;
}

rad& rad::operator/= (rad amount)
{
    radian = radian / amount;

    radian = constrain_rad(radian);

    return *this;
}