
#pragma once

#ifndef POS_H
#define POS_H

#include <cmath>
#include <iostream>

#include <box2d/b2_math.h>

struct pos
{
public:
    double x = 0;
    double y = 0;

    pos(double new_x, double new_y);

    pos();

    pos(const pos& past_pos);

    double size();

    double direction();

    pos ratio();

    double len();

    pos rotated(double angle);

    double angle_to(pos target);

    pos limited(double limit);

    void stop();

    pos scaled(pos start, pos end);

    bool within(pos a, pos b);

    pos floor();

    pos ceil();

    pos round();

    b2Vec2 to_b2Vec2();

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


#endif
