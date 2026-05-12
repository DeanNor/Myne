
#include "chudette.hpp"

#include "chud_boss_manager.hpp"

Chuds::Chuds(sprite* chudspr, ChudManager* _manager, bool first) : Damage(100, 100), manager(_manager), max_health(health)
{
    DrawObj* spr = new DrawObj;
    add_child(spr);

    spr->set_sprite(chudspr, false);
    spr->set_depth(67);
    spr->set_scale({1/40., 1/40.});

    pos spr_size = spr->get_texture()->half_size * spr->get_scale();

    collision_def.type = b2_dynamicBody;
    collision_def.enableSleep = false;

    collision_body = b2CreateBody(get_current_coll_world(), &collision_def);

    b2Polygon box = b2MakeBox(spr_size.x , spr_size.y);
    
    b2ShapeDef fixtureDef = b2DefaultShapeDef();
    fixtureDef.density = 0.5;
    fixtureDef.material.friction = 0;
    fixtureDef.material.restitution = 1;
    fixtureDef.enableContactEvents = true;

    fixtureDef.filter.categoryBits = MASK::PLANE;
    fixtureDef.filter.maskBits = MASK::BULLET | MASK::WALL;

    b2CreatePolygonShape(collision_body, &fixtureDef, &box);

    impulse = b2Body_GetRotationalInertia(collision_body);

    marker = new Marker(SPR::emarker, this, MDIST::EMARKER);
    marker->set_depth(201);

    if (first)
    {
        healthbar = new Healthbar("Chud", {-250, -910}, true);
        manager->add_child(healthbar);
        healthbar->set_depth(210);
        healthbar->set(1);
    }

    else
    {
        healthbar = new Healthbar("Foid", {-750, -910}, false);
        manager->add_child(healthbar);
        healthbar->set_depth(210);
        healthbar->set(1);
    }
}

void Chuds::collide_begin(CollObj* other)
{
    Damage* damage = dynamic_cast<Damage*>(other);

    if (damage)
    {
        health -= damage->get_damage();

        if (health <= 0)
        {
            marker->start_delete();
            start_delete();

            healthbar->set(0);

            manager->enrage(this);
        }

        else
        {
            healthbar->set(health / max_health);
        }
    }
}