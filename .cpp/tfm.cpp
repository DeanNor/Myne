
#include ".hpp/tfm.hpp"

void tfm::set(pos value)
{
    if (parent != nullptr)
    {
            *position = value - parent->compute();
    }

    else *position = value;
}

// TODO: replace recursive calls to angle_changed and has_changed.
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

        if (parent->scale_changed())
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
        if (parent != nullptr)
        {
            par_pos = parent->compute();
            transform = position->rotated(parent->compute_angle());

            if (parent->scale != nullptr)
            {
                transform += *(parent->scale) * par_pos;
            }
            
            else transform += par_pos;
        }

        else
        {
            transform = *position;
        }

        if (scale) past_scale = *scale;

        past_pos = *position;
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
        if (parent != nullptr)
        {
            transform_angle = *angle;
            past_angle = transform_angle;
            
            par_angle = parent->compute_angle();
            transform_angle += par_angle;
        }

        else
        {
            transform_angle = *angle;
            past_angle = transform_angle;
        }
    }

    return transform_angle;
}

bool tfm::scale_changed()
{
    if (scale) return past_scale == *scale;

    return false;
}

void tfm::deparent()
{
    transform = *position;
    transform_angle = *angle;

    parent = nullptr;
}