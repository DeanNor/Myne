
#pragma once

#ifndef OBJ_H
#define OBJ_H

#include "process.hpp"

#include "tfm.hpp"

#include <vector>

class Object: public Process
{
protected:
    pos position;
    rad angle = 0;

    tfm global_position = {&position, &angle};

public:
    void set_parent(Process* new_parent);

    tfm* get_tfm();

    pos get_global_position();

    void set_position(pos new_pos);

    pos get_position();

    void set_angle(rad new_angle);

    rad get_angle();

    rad get_global_angle();
};

#endif