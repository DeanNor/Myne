
#pragma once

#include "pos.hpp"

struct tfm
{
private:
    // Storage of past values for quick compute
    pos past_pos;
    rad past_angle = 0;

    pos par_pos;
    rad par_angle = 0;

public:
    // Internal values to quick compute transform and angle.
    // Should not normally be used.
    pos transform;
    rad transform_angle;

    // Un-Owned position and angle variables, stored in the object class but not here.
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

    // Gives the position.
    // Technically, it is better to store this that to compute it twice.
    pos compute();

    bool angle_changed();

    // Gives the angle
    rad compute_angle();
};
