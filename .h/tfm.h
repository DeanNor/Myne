
#pragma once

#include "pos.h"

struct tfm
{
public:
    pos transform;
    double transform_angle = 0;

    pos past_pos;
    double past_angle = 0;

    pos past_parent;

public:
    pos* position = nullptr;
    double* angle = nullptr;

    tfm* parent = nullptr;

    tfm(pos* new_position, double* new_angle);
    tfm(pos* new_position, double* new_angle, tfm* new_parent);

    bool has_changed();

    pos compute();

    bool angle_changed();

    double compute_angle();

    void eof();
};
