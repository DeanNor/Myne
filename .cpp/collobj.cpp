
#include "collobj.hpp"

CollObj::CollObj()
{
    get_current_game()->collisions.push_back(this);

    collision_def = b2DefaultBodyDef();
    collision_def.userData = this;
}

CollObj::~CollObj()
{
    b2DestroyBody(collision_body);

    std::vector<CollObj*>& collisions = get_current_game()->collisions;

    std::vector<CollObj*>::iterator index = std::find(collisions.begin(), collisions.end(), this);

    if (index != collisions.end())
    {
        collisions.erase(index);
    }

    else
    {
        std::cout << "HUH Collision" << std::endl; // Error, object seems already deleted and has no draw calls
    }
}

void CollObj::process(double delta)
{
    Object::process(delta);

    b2Body_SetTransform(collision_body,global_position.compute(),global_position.compute_angle());
}

void CollObj::collision_process()
{
    b2Vec2 body_b2pos = b2Body_GetPosition(collision_body);
    pos body_pos(body_b2pos.x, body_b2pos.y);
    pos offset = {0, 0};
    
    if (global_position.parent != nullptr)
    {        
        offset = *global_position.parent;
    }
    
    position = body_pos - offset;

    angle = b2Body_GetRotation(collision_body);
}

void CollObj::collide_with(CollObj* other)
{
    return;
}