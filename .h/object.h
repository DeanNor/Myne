
#pragma once

#ifndef OBJ_H
#define OBJ_H

#include "process.h"

#include <vector>

class Object: public Process
{
protected:
    pos position;
    double angle = 0;

    pos global_position;
    double global_angle = 0;

public:
    void process(double delta);

    pos get_offset();

    pos get_global_position();

    void set_position(pos new_pos);

    pos get_position();

    void set_angle(double new_angle);

    double get_angle();

    double get_global_angle();
};

#endif