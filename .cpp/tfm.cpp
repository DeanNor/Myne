
#include ".hpp/tfm.hpp"
#include ".hpp/pos.hpp"

void tfm::set(pos value)
{
    if (parent)
    {
        *position = ((value - parent->compute()) / parent->compute_scale()).rotated(-(parent->compute_angle()));
    }

    else *position = value;
}

void tfm::set_angle(rad value)
{
    if (parent)
    {
        *angle = (value - parent->compute_angle());
    }

    else *angle = value;
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
        if (parent->transform != par_pos)
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

            if (parent->scale != nullptr)
            {
                transform = position->rotated(parent->compute_angle()) * *(parent->scale);
            }
            
            else
            {
                transform = position->rotated(parent->compute_angle());
            }

            transform += par_pos;
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
        if (parent->transform_angle != par_angle)
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
    if (!scale) return false;
    
    if(past_scale != *scale)
    {
        return true;
    }

    if (parent) return parent->scale_changed();

    return false;
}

pos tfm::compute_scale()
{
    pos transform_scale;
    
    if (scale) transform_scale = *scale;
    else transform_scale = {1,1};

    if (parent)
    {
        transform_scale *= parent->compute_scale();
    }

    return transform_scale;
}

void tfm::deparent()
{
    transform = *position;
    transform_angle = *angle;

    parent = nullptr;
}