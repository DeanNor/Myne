
#pragma once

#include <box2d/math_functions.h>

#include "loader.hpp"
#include "saver.hpp"

#include <cmath>

// Radian type, auto looping
struct rad
{
ASSIGN_VAR_CONSTRUCTOR(rad);

private:
    constexpr static const double _PI = 3.14159265358979323846;
    constexpr static const double TO_DEG = 180.0 / _PI;

    constexpr double rad_constraint(const double amount) const
    {
        if (amount > 0)
        {
            return std::fmod(amount, _PI) - _PI;
        }
        
        return std::fmod(amount, _PI) + _PI;
    }

    constexpr double constrain_rad(const double amount) const
    {
        if (amount > _PI || amount <= -_PI)
        {
            return rad_constraint(amount);
        }

        return amount;
    }

    constexpr rad constrain_rad(const rad amount) const
    {
        if (amount.radian > _PI || amount.radian <= -_PI)
        {
            return rad_constraint(amount.radian);
        }

        return amount.radian;
    }

public:
    double radian;

    static constexpr rad PI()
    {
        return rad(_PI);
    }

    static constexpr double DEG_CONV_CONST()
    {
        return TO_DEG;
    }

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
    constexpr bool operator> (rad compare) const
    {
        return constrain_rad(radian - compare.radian) > 0;
    }

    constexpr bool operator< (rad compare) const
    {
        return constrain_rad(radian - compare.radian) < 0;
    }

    constexpr bool operator>= (rad compare) const
    {
        return constrain_rad(radian - compare.radian) >= 0;
    }

    constexpr bool operator<= (rad compare) const
    {
        return constrain_rad(radian - compare.radian) <= 0;
    }

    constexpr bool operator== (rad compare) const
    {
        return radian == compare.radian;
    }

    constexpr bool operator!= (rad compare) const
    {
        return radian != compare.radian;
    }

    // ----- +
    constexpr rad operator+ (rad amount) const
    {
        return amount.radian + radian;
    }

    constexpr rad operator+ (double amount) const
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
    constexpr rad operator- (rad amount) const
    {
        return radian - amount.radian;
    }

    constexpr rad operator- (double amount) const
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
    constexpr rad operator* (rad amount) const
    {
        return amount.radian * radian;
    }

    constexpr rad operator* (double amount) const
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
    constexpr rad operator/ (rad amount) const
    {
        return amount.radian / radian;
    }

    constexpr rad operator/ (double amount) const
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
constexpr rad drad(const double degree)
{
    return rad(degree / rad::DEG_CONV_CONST());
}

constexpr rad operator ""_r(const long double v)
{
    return rad(v);
}

constexpr long double operator ""_d(const long double v)
{
    return drad(v);
}

constexpr rad operator ""_r(const unsigned long long v)
{
    return rad(v);
}

constexpr long double operator ""_d(const unsigned long long v)
{
    return drad(v);
}