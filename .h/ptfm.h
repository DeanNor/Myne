
#pragma once

#include "tfm.h"

struct ptfm : public tfm
{
public:
    tfm* parent = nullptr;

    ptfm(pos* new_position, double* new_angle, tfm* parent_tfm);

    pos calculate();
};