
#include "object.hpp"

void Object::set_parent(Process* new_parent)
{
    Process::set_parent(new_parent);

    Object* p = dynamic_cast<Object*>(new_parent);

    if (p != nullptr)
    {
        global_position.parent = p->get_tfm();
    }
}

tfm* Object::get_tfm()
{
    return &global_position;
}

pos Object::get_global_position()
{
    return global_position.compute();
}

void Object::set_position(pos new_pos)
{
    position = new_pos;
}

pos Object::get_position()
{
    return position;
}

void Object::set_angle(rad new_angle)
{
    angle = new_angle;
}

rad Object::get_angle()
{
    return angle;
}

rad Object::get_global_angle()
{
    return global_position.compute_angle();
}
