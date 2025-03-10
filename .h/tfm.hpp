
#pragma once

#include "pos.hpp"
#include "rad.hpp"

struct tfm
{
private:
    pos transform;
    rad transform_angle = 0;

    pos past_pos;
    rad past_angle = 0;

    pos past_parent;
    rad past_par_angle = 0;

public:
    pos* position = nullptr;
    rad* angle = nullptr;

    tfm* parent = nullptr; // Parent transform obj, also a good notifier if the parent is an object instead of a process

    tfm(pos* new_position, rad* new_angle);
    tfm(pos* new_position, rad* new_angle, tfm* new_parent);

    bool has_changed();

    pos compute();

    bool angle_changed();

    rad compute_angle();

    operator pos();
};
