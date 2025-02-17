
#pragma once

#include "pos.h"

struct tfm
{
protected:
    pos past_pos;
    double past_angle = 0;

    pos transform;

public:
    pos* position = nullptr;
    double* angle = nullptr;

    tfm(pos* new_position, double* new_angle);

    void update_transform();

    virtual pos calculate();
};