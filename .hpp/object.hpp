
#pragma once

#include ".hpp/factory.hpp"
#include ".hpp/process.hpp"

#include ".hpp/tfm.hpp"

class Object: public Process
{
ASSIGN_CONSTRUCTOR(Object)

protected:
    pos position = {0,0};
    rad angle = {0};

    tfm global_position = {&position, &angle};

public:
    virtual void load(Loader* load) override;

    virtual void save(Saver* save) const override;

    void set_parent(Process* new_parent) override;

    tfm* get_tfm();

    pos get_global_position();

    void set_position(pos new_pos);

    pos get_position();

    void set_angle(rad new_angle);

    rad get_angle();

    rad get_global_angle();
};

class OBJ2_0 : public Object
{
ASSIGN_CONSTRUCTOR(OBJ2_0)
};

class OBJFAIL_0 : public Object
{

};