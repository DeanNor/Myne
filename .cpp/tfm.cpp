
#include "tfm.h"

tfm::tfm(pos* new_position, double* new_angle) : position(new_position), angle(new_angle)
{
    update_transform();
}

void tfm::update_transform()
{
    transform = position->rotated(*angle);

    past_pos = *position;
    past_angle = *angle;
}

pos tfm::calculate()
{
    if (*position != past_pos && *angle != past_angle)
    {
        update_transform();
    }

    return transform;
}