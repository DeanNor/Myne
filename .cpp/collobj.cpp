
#include "collobj.h"

CollObj::CollObj()
{
    get_current_game()->collisions.push_back(this);
}

void CollObj::process(double delta)
{
    Object::process(delta);
    
    if (global_position != past_offset)
    {
        past_offset = global_position;

        collision_body->SetTransform(past_offset.to_b2Vec2(),global_angle);
    }
}

void CollObj::collision_process()
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
