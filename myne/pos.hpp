
#pragma once

#include "b2.h"
#include <SDL3/SDL_render.h>

#include "rad.hpp"

#include "saver.hpp"
#include "loader.hpp"

// TODO efficiency week! aka constexpr members
// I dont know why I keep assuming I initialize this, it is a type, initialize it yourself!
struct pos
{
ASSIGN_VAR_CONSTRUCTOR(pos);

public:
    double x;
    double y;
        
    constexpr pos(const double new_x, const double new_y) : x(new_x), y(new_y) {}

    constexpr pos() = default;

    constexpr operator b2Vec2() { return b2Vec2{(float)x,(float)y}; }

    constexpr pos(const b2Vec2& convert) : x(convert.x), y(convert.y) {}

    constexpr operator SDL_FPoint() { return SDL_FPoint{(float)x,(float)y}; }

    constexpr pos(const SDL_FPoint& convert) : x(convert.x), y(convert.y) {};

    explicit pos(SDL_Texture* a)
    {
        float f_x, f_y;

        SDL_GetTextureSize(a, &f_x, &f_y);

        x = f_x;
        y = f_y;
    }

    static constexpr SDL_FRect Make_SDL_FRect(const pos& center, const pos& offset)
    {
        return SDL_FRect{(float)(center.x - offset.x), (float)(center.y - offset.y), (float)(offset.x * 2.0f), (float)(offset.y * 2.0f)};
    }

    static constexpr SDL_Rect Make_SDL_Rect(const pos& center, const pos& offset)
    {
        return SDL_Rect{(int)(center.x - offset.x), (int)(center.y - offset.y), (int)(offset.x * 2.0f), (int)(offset.y * 2.0f)};
    }

    void load(Loader* load)
    {
        x = load->load_data<double>();
        y = load->load_data<double>();
    }

    void save(Saver* save) const
    {
        save->save_data(x);
        save->save_data(y);
    }

    constexpr double sum() const
    {
        return std::abs(x) + std::abs(y);
    }

    constexpr rad direction() const
    {
        return rad::force(std::atan2(y,x));
    }

    // Percent length for x and y. (10,5) -> (0.66,0.33)
    constexpr pos ratio() const
    {
        const double scale = sum();

        if (scale != 0)
        {
            pos x_to_y;

            x_to_y.x = x / scale;
            x_to_y.y = y / scale;

            return x_to_y;
        }

        else
        {
            return {0,0};
        }
    }

    // Normalized length for x and y. (10, 5) -> (0.894428,0.447214)
    constexpr pos normal() const
    {
        const double scale = len();

        if (scale != 0)
        {
            pos x_to_y;

            x_to_y.x = x / scale;
            x_to_y.y = y / scale;

            return x_to_y;
        }

        else
        {
            return {0,0};
        }
    }

    // Hypotenuse distance
    constexpr double len() const
    {
        return std::sqrt(std::pow(x,2) + std::pow(y,2));
    }

    pos rotated(const rad angle) const
    {
        if (angle != rad(0.0))
        {
            const double cosine = std::cos(angle);
            const double sine = std::sin(angle);

            pos temp_pos;

            temp_pos.x = (x * cosine) - (y * sine);

            temp_pos.y = (x * sine) + (y * cosine);

            return temp_pos;
        }

        else
        {
            return *this;
        }
    }

    constexpr rad angle_to(const pos& target) const
    {
        return rad::force(std::atan2(target.y - y,target.x - x));
    }

    // Limited by length, as in total pos offset length
    constexpr pos limited(const double limit) const
    {
        const double length = len();
        pos temp_pos = *this;

        if (length > limit)
        {
            temp_pos = normal() * limit;
        }

        return temp_pos;
    }

    // Limited by x length and y length as separate values not affecting each other
    constexpr pos limited_separated(const double limit) const
    {
        pos temp_pos = *this;
        
        if (std::abs(x) > limit)
        {
            temp_pos.x = std::copysign(limit,x);
        }

        if (std::abs(y) > limit)
        {
            temp_pos.y = std::copysign(limit,y);
        }

        return temp_pos;
    }

    constexpr pos scaled(const pos& start, const pos& end) const
    {
        pos temp_pos = *this;

        temp_pos *= end / start;
        
        return temp_pos;
    }

    constexpr bool within(const pos& min, const pos& max) const
    {
        if (x < min.x || x > max.x)
        {
            return false;
        }

        if (y < min.y || y > max.y)
        {
            return false;
        }

        return true;
    }

    constexpr pos floor() const
    {
        pos temp_pos = *this;

        temp_pos.x = std::floor(temp_pos.x);
        temp_pos.y = std::floor(temp_pos.y);

        return temp_pos;
    }

    constexpr pos ceil() const
    {
        pos temp_pos = *this;

        temp_pos.x = std::ceil(temp_pos.x);
        temp_pos.y = std::ceil(temp_pos.y);

        return temp_pos;
    }

    constexpr pos round() const
    {
        pos temp_pos = *this;

        temp_pos.x = std::round(temp_pos.x);
        temp_pos.y = std::round(temp_pos.y);

        return temp_pos;
    }

    constexpr double distance_to(const pos& where) const
    {
        return std::sqrt(std::pow(where.x - x, 2) + std::pow(where.y - y, 2));
    }

    // Round down to nearest multiple of tile_size
    constexpr pos tilefy(const pos& tile_size) const
    {
        pos temp_pos = *this;

        temp_pos.x = std::round(x / tile_size.x) * tile_size.x;
        temp_pos.y = std::round(y / tile_size.y) * tile_size.y;

        return temp_pos;
    }

    // Largest of x or y.
    constexpr double large() const
    {
        return x > y ? x : y;
    }

    // Smallest of x or y.
    constexpr double small() const
    {
        return x < y ? x : y;
    }

    constexpr pos modulo(const pos& a) const
    {
        return {std::fmod(x, a.x), std::fmod(y,a.y)};
    }
    
    constexpr const pos& operator+() const
    {
        return *this;
    }

    constexpr pos operator+ (const pos& amount) const
    {
        pos temp_pos = *this;

        temp_pos.x += amount.x;
        temp_pos.y += amount.y;

        return temp_pos;
    }

    pos& operator+= (const pos& amount)
    {
        x += amount.x;
        y += amount.y;

        return *this;
    }

    void add_in_direction(const double amount, const rad angle)
    {
        x += amount * std::cos(angle);
        y += amount * std::sin(angle); 
    }

    constexpr pos operator- () const
    {
        return {-x,-y};
    }

    constexpr pos operator- (const pos& amount) const
    {
        pos temp_pos = *this;

        temp_pos.x -= amount.x;
        temp_pos.y -= amount.y;

        return temp_pos;
    }

    pos& operator-= (const pos& amount)
    {
        x -= amount.x;
        y -= amount.y;

        return *this;
    }

    constexpr pos operator* (const pos& amount) const
    {
        pos temp_pos = *this;

        temp_pos.x *= amount.x;
        temp_pos.y *= amount.y;

        return temp_pos;
    }

    constexpr pos operator* (const double amount) const
    {
        pos temp_pos = *this;

        temp_pos.x *= amount;
        temp_pos.y *= amount;

        return temp_pos;
    }

    pos& operator*= (const pos& amount)
    {
        x *= amount.x;
        y *= amount.y;

        return *this;
    }

    pos& operator*= (const double amount)
    {
        x *= amount;
        y *= amount;

        return *this;
    }

    constexpr pos operator/ (const pos& amount) const
    {
        pos temp_pos = *this;
        temp_pos.x /= amount.x;
        temp_pos.y /= amount.y;

        return temp_pos;
    }

    constexpr pos operator/ (const double amount) const
    {
        pos temp_pos = *this;

        temp_pos.x /= amount;
        temp_pos.y /= amount;

        return temp_pos;
    }

    pos& operator/= (const pos& amount)
    {
        x /= amount.x;
        y /= amount.y;

        return *this;
    }

    pos& operator/= (const double amount)
    {
        x /= amount;
        y /= amount;

        return *this;
    }

    constexpr bool operator== (const pos& to_compare) const
    {
        if (x == to_compare.x && y == to_compare.y)
        {
            return true;
        }

        return false;
    }

    constexpr bool operator!= (const pos& to_compare) const
    {
        if (*this == to_compare)
        {
            return false;
        }

        return true;
    }
};

inline std::ostream& operator<< (std::ostream& os, const pos& convert)
{
    os << "X: " << convert.x << " Y: " << convert.y;
    return os;
}

namespace std
{
    inline constexpr pos copysign(const pos& v, double c)
    {
        return {std::copysign(v.x,c), std::copysign(v.y, c)};
    }

    inline constexpr pos copysign(const pos& v, const pos& c)
    {
        return {std::copysign(v.x, c.x), std::copysign(v.y,c.y)};
    }

    inline constexpr pos abs(pos v)
    {
        return {abs(v.x), abs(v.y)};
    }
}

inline pos& operator-= (const double a, pos& b)
{
    b.x = a - b.x;
    b.y = a - b.y;

    return b;
}

inline constexpr pos operator- (const double a, const pos& b)
{
    return pos{a - b.x, a - b.y};
}

inline pos& operator*= (const double a, pos& b)
{
    return b *= a;
}

inline constexpr pos operator* (const double a, const pos& b)
{
    return b * a;
}

inline pos& operator/= (const double a, pos& b)
{
    b.x = a / b.x;
    b.y = a / b.y;

    return b;
}

inline constexpr pos operator/ (const double a, const pos& b)
{
    return {a / b.x, a / b.y};
}