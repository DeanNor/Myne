
#pragma once

#include <cmath>
#include <iostream>

#include <box2d/math_functions.h>
#include <SDL3/SDL_rect.h>

#include "rad.hpp"

struct pos
{
public:
    double x = 0;
    double y = 0;

    pos(const double& new_x, const double& new_y);

    pos();

    pos(const pos& past_pos);

    operator b2Vec2();

    pos(const b2Vec2& convert);

    operator SDL_FPoint();

    pos(const SDL_FPoint& convert);

    static SDL_FRect make_SDL_FRect(const pos& center, const pos& offset);

    double size();

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