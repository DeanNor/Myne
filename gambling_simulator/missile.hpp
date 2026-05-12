
#pragma once

#include ".hpp/collobj.hpp"
#include ".hpp/drawobj.hpp"
#include ".hpp/game.hpp"
#include "box2d/box2d.h"
#include "box2d/collision.h"
#include "gambling_simulator/damage.hpp"
#include "gambling_simulator/player.hpp"
#include "gambling_simulator/sprite_extra.hpp"
#include <cmath>

class Miata : public Damage
{
    static constexpr double lin_speed = 300;
    static constexpr double turn_speed = 300;

    double lifespan = 5;

    float impulse = 120;

    Marker* marker;

public:
    Miata() : Damage(5,1)
    {
        DrawObj* draw = new DrawObj;
        add_child(draw);

        draw->set_sprite(SPR::missile_sprite, false);
        draw->set_depth(199);
        draw->set_scale({1/4., 1/4.});

        pos spr_size = draw->get_texture()->half_size * draw->get_scale();

        collision_def.type = b2_dynamicBody;
        collision_def.enableSleep = false;

        collision_body = b2CreateBody(get_current_coll_world(), &collision_def);

        b2Polygon box = b2MakeBox(spr_size.x , spr_size.y);
        
        b2ShapeDef fixtureDef = b2DefaultShapeDef();
        fixtureDef.density = 0.5;
        fixtureDef.material.friction = 0;
        fixtureDef.material.restitution = 1;
        fixtureDef.enableContactEvents = true;

        fixtureDef.filter.categoryBits = MASK::MISSILE;
        fixtureDef.filter.maskBits = MASK::BULLET | MASK::PLANE | MASK::WALL;

        b2CreatePolygonShape(collision_body, &fixtureDef, &box);
        b2Body_SetMassData(collision_body, b2MassData{0.7, spr_size, impulse});

        angle = position.angle_to(get_player()->get_position());

        marker = new Marker(SPR::mmarker, this, MDIST::MMARKER);
        marker->set_depth(200);
    }

    void explode()
    {
        Explosion* s = new Explosion(position, SPR::missile_break);
        s->set_angle(rad::PI() / 2. + angle);
        s->set_depth(160);

        parent->add_child(s);

        marker->start_delete();

        start_delete();
    }

    virtual void collision_process() override
    {
        CollObj::collision_process();

        Object* player = get_player();
        double distance = (player->get_position() - position).len();
        rad angle_to_target = position.angle_to(player->get_position() + (PLAYER_VEL * (distance / lin_speed)));

        if (angle + (std::pow(rot_velocity, 2) / (2. * turn_speed / impulse)) * (std::signbit(rot_velocity) ? -1 : 1) <= angle_to_target) 
        {
            b2Body_ApplyTorque(collision_body, turn_speed, true);
        }

        else
        {
            b2Body_ApplyTorque(collision_body, -turn_speed, true);
        }

        lin_velocity = pos{lin_speed, 0}.rotated(angle);

        lifespan -= get_current_game()->get_delta();

        if (lifespan <= 0)
        {
            explode();
        }
    }

    virtual void collide_begin(CollObj* other) override
    {
        explode();
    }
};