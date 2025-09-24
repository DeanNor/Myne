
#pragma once

#include "pos.hpp"

struct tfm
{
private:
    // Storage of past values for quick compute
    pos past_pos = {0,0};
    rad past_angle = {0};

    pos par_pos = {0,0};
    rad par_angle = {0};

public:
    // Internal values to quick compute transform and angle.
    // Should not normally be used.
    pos transform = {0,0};
    rad transform_angle = 0;

    // Un-Owned position and angle variables, stored in the object class but not here.
    pos* position = nullptr;
    rad* angle = nullptr;

    tfm* parent = nullptr; // Parent transform obj, also a good notifier if the parent is an object (and acting like one) instead of a process

    tfm(pos* new_position, rad* new_angle);
    tfm(pos* new_position, rad* new_angle, tfm* new_parent);

    // Set the underlying position to value, taking into account the global position the underlying position is scoped to.
    void set(pos value);

    bool has_changed();

    // Gives the position.
    // Technically, it is better to store this that to compute it twice.
    pos compute();

    bool angle_changed();

    // Gives the angle
    rad compute_angle();

    void deparent();
};
