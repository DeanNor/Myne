
#include "collobj.h"

CollObj::CollObj()
{
    get_current_game()->collisions.push_back(this);
}

void CollObj::process(double delta)
{
    Object::process(delta);
    
    pos computed = global_position.compute();
    if (computed != past_offset)
    {
        past_offset = computed;

        collision_body->SetTransform(past_offset.to_b2Vec2(),global_position.compute_angle());
    }
}

void CollObj::collision_process(double delta)
{
    b2Vec2 body_pos = collision_body->GetPosition();
    pos offset = {};
    
    if (parent != nullptr)
    {
        Object* p = dynamic_cast<Object*>(parent);
        
        if (p != nullptr)
        {
            offset = p->get_global_position();
        }
    }
    
    position.x = body_pos.x - offset.x;
    position.y = body_pos.y - offset.y;

    angle = collision_body->GetAngle();
}
