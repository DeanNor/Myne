
#include "object.h"

void Object::process(double delta)
{
    if (parent != nullptr)
    {
        double p_ang = parent->get_global_angle();
        global_position = parent->get_global_position() + position.rotated(p_ang);
        
        global_angle = p_ang + angle;
    }

    else
    {
        global_position = position;
        global_angle = angle;
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
