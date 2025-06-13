
#pragma once

#include "pos.hpp"
#include "rad.hpp"

struct tfm
{
private:
    // Internal values to quick compute transform and angle
    pos transform;
    rad transform_angle = 0;

    // Storage of past values for quick compute
    pos past_pos;
    rad past_angle = 0;

    pos par_pos;
    rad par_angle = 0;

public:
    pos* position = nullptr;
    rad* angle = nullptr;

    tfm* parent = nullptr; // Parent transform obj, also a good notifier if the parent is an object instead of a process

    tfm(pos* new_position, rad* new_angle);
    tfm(pos* new_position, rad* new_angle, tfm* new_parent);

    // Very basic math functions, the position should be separated for anything else, see efficiency rant
    void add(pos other);
    void subt(pos other);
    void mult(pos other);
    void div(pos other);

    void set(pos value);

    bool has_changed();

    pos compute();

    bool angle_changed();

    rad compute_angle();
};
