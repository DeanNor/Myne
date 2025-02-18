
#pragma once

#ifndef OBJ_H
#define OBJ_H

#include "process.h"

#include "tfm.h"

#include <vector>

class Object: public Process
{
protected:
    pos position;
    double angle = 0;

    tfm global_position = {&position, &angle};

public:
    void set_parent(Process* new_parent);

    tfm* get_tfm();

    pos get_global_position();

    void set_position(pos new_pos);

    pos get_position();

    void set_angle(double new_angle);

    double get_angle();

    double get_global_angle();
};

#endif