
#include "ptfm.h"

ptfm::ptfm(pos* new_position, double* new_angle, tfm* parent_tfm) : tfm(new_position, new_angle), parent(parent_tfm)
{

}

pos ptfm::calculate()
{
    tfm::calculate();

    if (parent != nullptr)
    {
        transform += parent->calculate();
    }

    return transform;
}