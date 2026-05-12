
#pragma once

#include ".hpp/collobj.hpp"
#include ".hpp/game.hpp"
#include ".hpp/sprite.hpp"
#include "box2d/box2d.h"
#include "gambling_simulator/damage.hpp"
#include "gambling_simulator/mask.hpp"
#include "gambling_simulator/sprite_extra.hpp"

class Bullet : public Damage
{
    double lifespan;
    static constexpr pos shot_vel = {500, 0};
public:
    void add_vel(pos initial_vel)
    {
        lin_velocity = initial_vel + shot_vel.rotated(angle);
    }

    Bullet(sprite* sprite) : Damage(10,1)
    {
        lifespan = (rand() % 200) / 100. + 1;

        Directional* spr = new Directional;

        spr->set_sprite(sprite, false);

        spr->set_depth(110);

        add_child(spr);

        pos spr_size = spr->get_texture()->half_size * spr->get_scale();

        collision_def.type = b2_dynamicBody;
        collision_def.enableSleep = false;
        collision_def.angularDamping = 0.5;

        collision_body = b2CreateBody(get_current_coll_world(), &collision_def);

        b2Polygon box = b2MakeBox(spr_size.x , spr_size.y);
        
        b2ShapeDef fixtureDef = b2DefaultShapeDef();
        fixtureDef.density = 0.5;
        fixtureDef.material.friction = 0;
        fixtureDef.material.restitution = 1;
        fixtureDef.filter.categoryBits = MASK::BULLET;
        fixtureDef.filter.maskBits = MASK::PLANE | MASK::MISSILE | MASK::WALL;

        b2CreatePolygonShape(collision_body, &fixtureDef, &box);

        b2Body_SetMassData(collision_body, {0.6,  spr_size, 340});
    }

    virtual void collision_process() override
    {
        lifespan -= get_current_game()->get_delta();

        if (lifespan <= 0)
        {
            start_delete();
        }
    }

    virtual void collide_begin(CollObj*) override
    {
        start_delete();
    }
};
