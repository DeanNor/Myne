
#pragma once

#include "factory.hpp"
#include "process.hpp"

#include "tfm.hpp"

class Object: public Process
{
ASSIGN_CONSTRUCTOR(Object)

protected:
    pos position;
    rad angle = 0;

    tfm global_position = {&position, &angle};

public:
    virtual void load(Loader* load);

    virtual void save(Saver* save) const;

    void set_parent(Process* new_parent);

    tfm* get_tfm();

    pos get_global_position();

    void set_position(pos new_pos);

    pos get_position();

    void set_angle(rad new_angle);

    rad get_angle();

    rad get_global_angle();
};