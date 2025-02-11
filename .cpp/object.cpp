
#include "object.h"

void Object::process(double delta)
{
    global_position = position;
    global_angle = angle;

    if (parent != nullptr)
    {
        global_position += parent->get_global_positio).rotated(angle);
        
        global_angle += parent->get_global_angle();
    }

    Process::process(delta);
}

pos Object::get_offset()
{
    pos offset = position;

    if (parent != nullptr)
    {
        offset = offset.rotated(parent->get_angle());

        offset += parent->get_offset();
    }

    return offset;
}

pos Object::get_global_position()
{
    return global_position;
}

void Object::set_position(pos new_pos)
{
    position = new_pos;
}

pos Object::get_position()
{
    return position;
}

void Object::set_angle(double new_angle)
{
    angle = new_angle;
}

double Object::get_angle()
{
    return angle;
}

double Object::get_global_angle()
{
    return global_angle;
}
