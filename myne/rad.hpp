
#pragma once

#include "loader.hpp"
#include "saver.hpp"

#include <cmath>
#include <iostream>

// Radian type, auto looping. Bounded by [-pi,pi] inclusive
struct rad
{
ASSIGN_VAR_CONSTRUCTOR(rad);

private:
    constexpr static const double _PI = 3.14159265358979323846;
    constexpr static const double TO_DEG = 180.0 / _PI;

    constexpr static double rad_constraint(const double amount)
    {
        int c = amount / _PI;

        if (c < 0)
        {
            if (c % 2 != 0) return std::fmod(amount, _PI) + _PI;
            return std::fmod(amount, _PI);
        }

        else
        {
            if (c % 2 != 0) return std::fmod(amount, _PI) - _PI;
            return std::fmod(amount, _PI);
        }
    }

public:
    constexpr static rad _constrain_rad(const rad amount)
    {
        if (amount.radian > _PI || amount.radian < -_PI)
        {
            return rad(rad::rad_constraint(amount.radian));
        }

        return amount;
    }

    constexpr static double _constrain_rad(const double amount)
    {
        if (amount > _PI || amount < -_PI)
        {
            return rad::rad_constraint(amount);
        }

        return amount;
    }

    double radian;

    static constexpr rad PI()
    {
        return rad(_PI);
    }

    static constexpr double PI_d()
    {
        return _PI;
    }

    static constexpr double DEG_CONV_CONST()
    {
        return TO_DEG;
    }

    constexpr double deg() const
    {
        return radian * TO_DEG;
    }

    explicit constexpr rad(const double& val) : radian(rad::rad::_constrain_rad(val)) {}

    constexpr rad(const b2Rot& val) : radian(b2Rot_GetAngle(val)) {}

    constexpr rad() = default;

    // Create rad without bound checks
    static constexpr rad force(const double& val)
    {
        rad v;
        v.radian = val;
        return v;
    }

    constexpr operator double() const
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

    // Returns current angle to the nearest chunk. Chunk size is designated by offset. Amount of chunks is in 2 * rad::PI() / amount
    rad nearest(double offset)
    {
        int multiple = std::round(radian / offset);

        return rad(offset * multiple);
    }

    // ----- ?
    constexpr bool operator> (rad compare) const
    {
        return rad::_constrain_rad(radian - compare.radian) > 0;
    }

    constexpr bool operator< (rad compare) const
    {
        return rad::_constrain_rad(radian - compare.radian) < 0;
    }

    constexpr bool operator>= (rad compare) const
    {
        return rad::_constrain_rad(radian - compare.radian) >= 0;
    }

    constexpr bool operator<= (rad compare) const
    {
        return rad::_constrain_rad(radian - compare.radian) <= 0;
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
    constexpr rad operator+() const
    {
        return *this;
    }

    constexpr rad operator+ (rad amount) const
    {
        return rad(amount.radian + radian);
    }

    constexpr rad operator+ (double amount) const
    {
        return rad(amount + radian);
    }

    constexpr rad& operator+= (rad amount)
    {
        radian = radian + amount.radian;

        radian = rad::_constrain_rad(radian);

        return *this;
    }

    constexpr rad& operator+= (double amount)
    {
        radian = radian + amount;

        radian = rad::_constrain_rad(radian);

        return *this;
    }

    // ----- -
    constexpr rad operator- () const
    {
        return rad(-radian);
    }

    constexpr rad operator- (rad amount) const
    {
        return rad(radian - amount.radian);
    }

    constexpr rad operator- (double amount) const
    {
        return rad(radian - amount);
    }

    constexpr rad& operator-= (rad amount)
    {
        radian = radian - amount.radian;

        radian = rad::_constrain_rad(radian);

        return *this;
    }

    constexpr rad& operator-= (double amount)
    {
        radian = radian - amount;

        radian = rad::_constrain_rad(radian);

        return *this;
    }

    // ----- *
    constexpr rad operator* (rad amount) const
    {
        return rad(amount.radian * radian);
    }

    constexpr rad operator* (double amount) const
    {
        return rad(radian * amount);
    }

    constexpr rad& operator*= (rad amount)
    {
        radian = radian * amount.radian;

        radian = rad::_constrain_rad(radian);

        return *this;
    }

    constexpr rad& operator*= (double amount)
    {
        radian = radian * amount;

        radian = rad::_constrain_rad(radian);

        return *this;
    }

    // ----- /
    constexpr rad operator/ (rad amount) const
    {
        return rad(amount.radian / radian);
    }

    constexpr rad operator/ (double amount) const
    {
        return rad(radian / amount);
    }

    constexpr rad& operator/= (rad amount)
    {
        radian = radian / amount.radian;

        radian = rad::_constrain_rad(radian);

        return *this;
    }

    constexpr rad& operator/= (double amount)
    {
        radian = radian / amount;

        radian = rad::_constrain_rad(radian);

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

constexpr rad operator ""_d(const long double v)
{
    return drad(v);
}

constexpr rad operator ""_r(const unsigned long long v)
{
    return rad(v);
}

constexpr rad operator ""_d(const unsigned long long v)
{
    return drad(v);
}