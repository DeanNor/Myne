
#include "collobj.h"

CollObj::CollObj()
{
    get_current_game()->collisions.push_back(this);
    collision_def.userData = this;
}

void CollObj::process(double delta)
{
    Object::process(delta);
    
    pos computed = global_position.compute();

    collision_body->SetTransform((computed / B2_SCALE).to_b2Vec2(),global_position.compute_angle());
}

void CollObj::collision_process(double delta)
{
    b2Vec2 body_b2pos = collision_body->GetTransform().p;
    pos body_pos(body_b2pos.x * B2_SCALE, body_b2pos.y * B2_SCALE);
    pos offset = {0, 0};
    
    if (global_position.parent != nullptr)
    {        
        offset = global_position.parent->compute();
    }
    
    position = body_pos - offset;

    angle = collision_body->GetAngle();
}
