
#include "tfm.hpp"

tfm::tfm(pos* new_position, rad* new_angle) : position(new_position), angle(new_angle), parent(nullptr)
{
    compute();
}
tfm::tfm(pos* new_position, rad* new_angle, tfm* new_parent) : position(new_position), angle(new_angle), parent(new_parent)
{
    compute();
}

bool tfm::has_changed()
{
    if (*position != past_pos || angle_changed())
    {
        return true;
    }

    if (parent != nullptr)
    {
        if (parent->transform != past_parent)
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
            transform = position->rotated(*parent->angle);

            past_parent = *parent;
            
            transform += past_parent;
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

    else if (parent != nullptr)
    {
        if (parent->transform_angle != past_par_angle)
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
            past_par_angle = parent->compute_angle();

            transform_angle += past_par_angle;
        }
    }

    return transform_angle;
}

tfm::operator pos()
{
    return compute();
}

tfm::operator rad()
{
    return compute_angle();
}

tfm::operator double()
{
    return (double)compute_angle();
}