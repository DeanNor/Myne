
#pragma once

#include ".hpp/collobj.hpp"

class Damage : public CollObj
{
protected:
    int damage;

    int health;
public:
    Damage(int _damage, int _health) : damage(_damage), health(_health)
    {

    }

    int get_damage()
    {
        return damage;
    }

    virtual void collide_begin(CollObj* other) override
    {
        Damage* damage = dynamic_cast<Damage*>(other);

        if (damage)
        {
            health -= damage->get_damage();

            if (health <= 0)
            {
                start_delete();
            }
        }
    }
};