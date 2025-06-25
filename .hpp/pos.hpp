
#pragma once

#include <ostream>

#include <box2d/math_functions.h>
#include <SDL3/SDL_rect.h>

#include "factory.hpp"
#include "rad.hpp"

#include "saver.hpp"
#include "loader.hpp"

// TODO efficiency week!
struct pos
{
ASSIGN_VAR_CONSTRUCTOR(pos)

public:
    double x;
    double y;
        
    constexpr pos(const double& new_x, const double& new_y) : x(new_x), y(new_y) {}

    constexpr pos() = default;

    constexpr pos(const pos& past_pos) : x(past_pos.x), y(past_pos.y){}

    constexpr operator b2Vec2()
    {
        return b2Vec2{(float)x,(float)y};
    }

    constexpr pos(const b2Vec2& convert)
    {
        x = convert.x;
        y = convert.y;
    }

    constexpr operator SDL_FPoint()
    {
        return SDL_FPoint{(float)x,(float)y};
    }

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

    double sum();

    rad direction();

    pos ratio();

    double len();

    pos rotated(rad angle);

    rad angle_to(pos target);

    pos limited(double limit);

    void stop();

    pos scaled(pos start, pos end);

    bool within(pos a, pos b);

    pos floor();

    pos ceil();

    pos round();

    pos& operator= (const pos& convert);

    pos operator+ (const pos amount);

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