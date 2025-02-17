
#pragma once

#include "process.h"

#include <vector>

#include "ptfm.h"

class Object: public Process
{
protected:
    pos position;
    double angle = 0;

    ptfm global_position = ptfm(&position, &angle, nullptr);
    double global_angle = 0;

public:
    void set_parent(Process* new_parent);

    ptfm& get_global_position();

    void set_position(pos new_pos);

    pos get_position();

    void set_angle(double new_angle);

    double get_angle();

    double get_global_angle();
};