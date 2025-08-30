
#pragma once

#include <box2d/math_functions.h>

#include "loader.hpp"
#include "saver.hpp"

const double PI = 3.14159265358979323846;
const double TO_DEG = 180.0 / PI;

// Radian type, auto looping
struct rad
{
ASSIGN_VAR_CONSTRUCTOR(rad);

private:
    constexpr double rad_constraint(double amount)
    {
        if (amount > 0)
        {
            return fmod(amount, PI) - PI;
        }
        
        return fmod(amount, PI) + PI;
    }

    constexpr double constrain_rad(double amount)
    {
        if (amount > PI || amount <= -PI)
        {
            return rad_constraint(amount);
        }

        return amount;
    }

    constexpr rad constrain_rad(rad amount)
    {
        if (amount.radian > PI || amount.radian <= -PI)
        {
            return rad_constraint(amount.radian);
        }

        return amount.radian;
    }

public:
    double radian;

    constexpr double deg() const
    {
        return radian * TO_DEG;
    }

    constexpr rad(const double& val) : radian(constrain_rad(val)) {}

    constexpr rad(const b2Rot& val) : radian(b2Rot_GetAngle(val)) {}

    constexpr rad() = default;

    constexpr operator double()
    {
        return radian;
    }

    constexpr operator b2Rot()
    {
        return b2MakeRot(radian);
    }

    void load(Loader* load)
    {
        radian = load->load_data<double>();
    }

    void save(Saver* save)
    {
        save->save_data(radian);
    }

    // ----- ?
    constexpr bool operator> (rad compare)
    {
        return constrain_rad(radian - compare.radian) > 0;
    }

    constexpr bool operator< (rad compare)
    {
        return constrain_rad(radian - compare.radian) < 0;
    }

    constexpr bool operator>= (rad compare)
    {
        return constrain_rad(radian - compare.radian) >= 0;
    }

    constexpr bool operator<= (rad compare)
    {
        return constrain_rad(radian - compare.radian) <= 0;
    }

    constexpr bool operator== (rad compare)
    {
        return radian == compare.radian;
    }

    constexpr bool operator!= (rad compare)
    {
        return radian != compare.radian;
    }

    // ----- +
    constexpr rad operator+ (rad amount)
    {
        return amount.radian + radian;
    }

    constexpr rad operator+ (double amount)
    {
        return rad(amount + radian);
    }

    constexpr rad& operator+= (rad amount)
    {
        radian = radian + amount;

        radian = constrain_rad(radian);

        return *this;
    }

    constexpr rad& operator+= (double amount)
    {
        radian = radian + amount;

        radian = constrain_rad(radian);

        return *this;
    }

    // ----- -
    constexpr rad operator- (rad amount)
    {
        return radian - amount.radian;
    }

    constexpr rad operator- (double amount)
    {
        return rad(radian - amount);
    }

    constexpr rad& operator-= (rad amount)
    {
        radian = radian - amount;

        radian = constrain_rad(radian);

        return *this;
    }

    constexpr rad& operator-= (double amount)
    {
        radian = radian - amount;

        radian = constrain_rad(radian);

        return *this;
    }

    // ----- *
    constexpr rad operator* (rad amount)
    {
        return amount.radian * radian;
    }

    constexpr rad operator* (double amount)
    {
        return rad(radian * amount);
    }

    constexpr rad& operator*= (rad amount)
    {
        radian = radian * amount;

        radian = constrain_rad(radian);

        return *this;
    }

    constexpr rad& operator*= (double amount)
    {
        radian = radian * amount;

        radian = constrain_rad(radian);

        return *this;
    }

    // ----- /
    constexpr rad operator/ (rad amount)
    {
        return amount.radian / radian;
    }

    constexpr rad operator/ (double amount)
    {
        return rad(radian / amount);
    }

    constexpr rad& operator/= (rad amount)
    {
        radian = radian / amount;

        radian = constrain_rad(radian);

        return *this;
    }

    constexpr rad& operator/= (double amount)
    {
        radian = radian / amount;

        radian = constrain_rad(radian);

        return *this;
    }
};

// Degree to radian
constexpr rad drad(double degree)
{
    return rad(degree / TO_DEG);
}

constexpr rad operator ""_r(long double v)
{
    return rad(v);
}

constexpr long double operator ""_d(long double v)
{
    return drad(v);
}

constexpr rad operator ""_r(unsigned long long v)
{
    return rad(v);
}

constexpr long double operator ""_d(unsigned long long v)
{
    return drad(v);
}