
#pragma once

#include ".hpp/collobj.hpp"
#include ".hpp/game.hpp"
#include "box2d/box2d.h"
#include "box2d/id.h"
#include "gambling_simulator/damage.hpp"
#include "gambling_simulator/sprite_extra.hpp"
#include "missile.hpp"
#include <cmath>

class ChudManager;

class Chuds : public Damage
{
    static constexpr double turn_speed = 6000;
    static constexpr pos max_vel = {300, 0};

    bool linear = false;

    double msl_wait = 2;
    double msl_timer = msl_wait;

    bool enraged = false;

    float impulse;

    ChudManager* manager;

    Marker* marker;

    double max_health;

    Healthbar* healthbar;

public:
    Chuds(sprite* chudspr, ChudManager* _manager, bool first);

    virtual void collision_process() override
    {
        Object* player = get_player();
        double distance = (player->get_position() - position).len();
        rad angle_to_target = position.angle_to(player->get_position() + (PLAYER_VEL * (distance / lin_velocity.len())));
        if (!linear)
        {
            if (angle + (std::pow(rot_velocity, 2) / (2. * turn_speed / impulse)) * (std::signbit(rot_velocity) ? -1 : 1) <= angle_to_target) 
            {
                b2Body_ApplyTorque(collision_body, turn_speed, true);
            }

            else
            {
                b2Body_ApplyTorque(collision_body, -turn_speed, true);
            }

            if (std::abs(angle - angle_to_target) < rad::PI_d() / 6.)
            {
                linear = true;
            }
        }

        else
        {
            b2Body_ApplyTorque(collision_body, turn_speed * (std::signbit(rot_velocity) ? 1 : -1), true);

            if (std::abs(angle - angle_to_target) > rad::PI_d() / 4.)
            {
                linear = false;
            }
        }

        msl_timer -= get_current_game()->get_delta();

        if (msl_timer <= 0)
        {
            if (enraged) 
            {
                Miata* en = new Miata;

                en->set_position(position + pos{0, 17}.rotated(angle));
                get_current_game()->get_root()->add_child(en);

                en->set_angle(angle + rad::PI() / 2.);

                Miata* en2 = new Miata;

                en2->set_position(position + pos{0, -17}.rotated(angle));
                get_current_game()->get_root()->add_child(en2);

                en2->set_angle(angle - rad::PI() / 2.);
            }

            else
            {
                Miata* en = new Miata;

                en->set_position(position + pos{-17, 0}.rotated(angle));
                get_current_game()->get_root()->add_child(en);

                en->set_angle(angle + rad::PI());
            }

            msl_timer = msl_wait;
        }

        lin_velocity = max_vel.rotated(angle);
    }

    void make_enraged()
    {
        enraged = true;

        msl_wait /= 2.;
    }

    virtual void collide_begin(CollObj* other) override;
};