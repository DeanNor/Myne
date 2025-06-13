
#include "tfm.hpp"

tfm::tfm(pos* new_position, rad* new_angle) : position(new_position), angle(new_angle), parent(nullptr)
{
    compute();
}
tfm::tfm(pos* new_position, rad* new_angle, tfm* new_parent) : position(new_position), angle(new_angle), parent(new_parent)
{
    compute();
}

// TODO incorrect math, mainly mult and div
void tfm::add(pos other)
{
    if (parent != nullptr)
    {
        pos par = parent->compute();
        *position = par - ((*position + par) + other);
    }

    else
    {
        *position += other;
    }
}
void tfm::subt(pos other)
{
    if (parent != nullptr)
    {
        pos par = parent->compute();
        *position = par - ((*position + par) - other);
    }

    else
    {
        *position -= other;
    }
}
void tfm::mult(pos other)
{
    if (parent != nullptr)
    {
        pos par = parent->compute();
        *position = par - ((*position + par) * other);
    }

    else
    {
        *position *= other;
    }
}
void tfm::div(pos other)
{
    if (parent != nullptr)
    {
        pos par = parent->compute();
        *position = par - ((*position + par) / other);
    }

    else
    {
        *position /= other;
    }
}

void tfm::set(pos value)
{
   if (parent) *position = value - parent->compute();
   else *position = value;
}

bool tfm::has_changed()
{
    if (*position != past_pos || angle_changed())
    {
        return true;
    }

    if (parent != nullptr)
    {
        if (par_pos != *parent->position)
        {
            return true;
        }

        return parent->has_changed();
    }

    return false;
}

pos tfm::compute()
{
    if (has_changed())
    {
        past_pos = *position;

        if (parent != nullptr)
        {
            transform = position->rotated(parent->compute_angle());
            
            par_pos = parent->compute();
            transform += par_pos;
        }

        else
        {
            transform = *position;
        }
    }

    return transform;
}

bool tfm::angle_changed()
{
    if (*angle != past_angle)
    {
        return true;
    }

    if (parent != nullptr)
    {        
        if (par_angle != *parent->angle)
        {
            return true;
        }

        return parent->angle_changed();
    }

    return false;
}

rad tfm::compute_angle()
{
    if (angle_changed())
    {
        past_angle = *angle;

        transform_angle = *angle;

        if (parent != nullptr)
        {
            par_angle = parent->compute_angle();

            transform_angle += par_angle;
        }
    }

    return transform_angle;
}
