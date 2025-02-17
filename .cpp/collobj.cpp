
#include "collobj.h"

CollObj::CollObj()
{
    get_current_game()->collisions.push_back(this);
}

void CollObj::process(double delta)
{
    Object::process(delta);
    
    pos calculated = global_position.calculate();
    if (calculated != past_offset)
    {
        past_offset = calculated;

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
            offset = p->get_global_position().calculate();
        }
    }

    std::cout << position << std::endl;
    
    position.x = body_pos.x - offset.x;
    position.y = body_pos.y - offset.y;
    std::cout << body_pos.x << ' ' << body_pos.y << std::endl;

    std::cout << position << " 2" << std::endl;

    angle = collision_body->GetAngle();
}
