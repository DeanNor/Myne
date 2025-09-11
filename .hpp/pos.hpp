
#pragma once

#include "box2d/math_functions.h"
#include "SDL3/SDL_rect.h"

#include "rad.hpp"

#include "saver.hpp"
#include "loader.hpp"

// TODO efficiency week!
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

    void load(Loader* load);

    void save(Saver* save);

    double sum() const;

    rad direction() const;

    pos ratio() const;

    double len() const;

    pos rotated(rad angle) const;

    rad angle_to(pos target) const;

    // Limited by length, as in total pos offset length
    pos limited(double limit) const;

    // Limited by x length and y length as separate values not affecting each other
    pos limited_separated(double limit) const;

    pos scaled(pos start, pos end) const;

    bool within(const pos& min, const pos& max) const;

    pos floor() const;

    pos ceil() const;

    pos round() const;
    
    pos operator+ (const pos amount); // TODO constexpr these

    pos operator+ (const double amount);

    pos& operator+= (const pos amount);

    pos& operator+= (const double amount);

    pos operator- (const pos amount);

    pos operator- (const double amount);

    pos& operator-= (const pos amount);

    pos& operator-= (const double amount);

    pos operator* (const pos amount);

    pos operator* (const double amount);

    pos& operator*= (const pos amount);

    pos& operator*= (const double amount);

    pos operator/ (const pos amount);

    pos operator/ (const double amount);

    pos& operator/= (const pos amount);

    pos& operator/= (const double amount);

    bool operator== (const pos& to_compare);

    bool operator!= (const pos& to_compare);

    friend std::ostream& operator<< (std::ostream& os, const pos& convert);
};