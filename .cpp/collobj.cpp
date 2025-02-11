
#include "collobj.h"

CollObj::CollObj()
{
    get_current_game()->collisions.push_back(this);
}

void CollObj::process(double delta)
{
    pos offset = get_offset();
    if (offset != past_offset)
    {
        past_offset = offset;

        collision_body->SetTransform(past_offset.to_b2Vec2(),angle);
    }

    Object::process(delta);
}

void CollObj::collision_process()
{
    b2Vec2 body_pos = collision_body->GetPosition();
    pos offset = get_parent_offset();
    
    position.x = body_pos.x - offset.x;
    position.y = body_pos.y - offset.y;

    angle = collision_body->GetAngle();
}