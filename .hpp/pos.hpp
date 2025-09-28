
#pragma once

#include "box2d/math_functions.h"
#include "SDL3/SDL_rect.h"

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
        
    constexpr pos(const double& new_x, const double& new_y) : x(new_x), y(new_y) {}

    constexpr pos() = default;

    constexpr operator b2Vec2() { return b2Vec2{(float)x,(float)y}; }

    constexpr pos(const b2Vec2& convert) : x(convert.x), y(convert.y) {}

    constexpr operator SDL_FPoint() { return SDL_FPoint{(float)x,(float)y}; }

    constexpr pos(const SDL_FPoint& convert) : x(convert.x), y(convert.y) {};

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
        rad dir = pos{0,0}.angle_to(*this);
        return dir;
    }

    // Normalized length. (10,5) -> (0.66,.33)
    constexpr pos ratio() const
    {
        pos x_to_y = {0,0};

        const double scale = sum();

        x_to_y.x = x / scale;
        x_to_y.y = y / scale;

        return x_to_y;
    }

    constexpr double len() const
    {
        return std::sqrt(std::pow(x,2) + std::pow(y,2));
    }

    pos rotated(rad angle) const
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

    constexpr rad angle_to(pos target) const
    {
        return atan2(target.y - y,target.x - x);
    }

    // Limited by length, as in total pos offset length
    constexpr pos limited(double limit) const
    {
        const double length = len();
        pos temp_pos = *this;

        if (length > limit)
        {
            temp_pos = ratio() * limit;
        }

        return temp_pos;
    }

    // Limited by x length and y length as separate values not affecting each other
    constexpr pos limited_separated(double limit) const
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

    constexpr pos scaled(pos start, pos end) const
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

    constexpr double distance_to(pos where) const
    {
        return std::sqrt(std::pow(where.x - x, 2) + std::pow(where.y - y, 2));
    }

    // Round down to nearest multiple of tile_size
    constexpr pos tilefy(pos tile_size) const
    {
        pos temp_pos = *this;

        temp_pos.x = std::round(x / tile_size.x) * tile_size.x;
        temp_pos.y = std::round(y / tile_size.y) * tile_size.y;

        return temp_pos;
    }

    constexpr pos operator+ (const pos amount) const
    {
        pos temp_pos = *this;
        temp_pos.x += amount.x;
        temp_pos.y += amount.y;

        return temp_pos;
    }

    constexpr pos operator+ (const double amount) const
    {
        const pos amount_over = ratio();
        pos temp_pos = *this;

        temp_pos.x += amount * amount_over.x;
        temp_pos.y += amount * amount_over.y;

        return temp_pos;
    }

    pos& operator+= (const pos amount)
    {
        x += amount.x;
        y += amount.y;

        return *this;
    }

    pos& operator+= (const double amount)
    {
        const pos amount_over = ratio();

        x += amount * amount_over.x;
        y += amount * amount_over.y;

        return *this;
    }

    constexpr pos operator- (const pos amount) const
    {
        pos temp_pos = *this;
        temp_pos.x -= amount.x;
        temp_pos.y -= amount.y;

        return temp_pos;
    }

    constexpr pos operator- (const double amount) const
    {
        const pos amount_over = ratio();
        pos temp_pos = *this;

        temp_pos.x -= amount * amount_over.x;
        temp_pos.y -= amount * amount_over.y;

        return temp_pos;
    }

    pos& operator-= (const pos amount)
    {
        x -= amount.x;
        y -= amount.y;

        return *this;
    }

    pos& operator-= (const double amount)
    {
        const pos amount_over = ratio();

        x -= amount * amount_over.x;
        y -= amount * amount_over.y;

        return *this;
    }


    constexpr pos operator* (const pos amount) const
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

    pos& operator*= (const pos amount)
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


    constexpr pos operator/ (const pos amount) const
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

    pos& operator/= (const pos amount)
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