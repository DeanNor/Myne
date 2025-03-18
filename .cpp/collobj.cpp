
#include "collobj.hpp"

struct BodyOwner : public b2BodyUserData
{
public:
    CollObj* owner;

    BodyOwner(CollObj* body_owner)
    {
        owner = body_owner;
    }
};

CollObj::CollObj()
{
    get_current_game()->collisions.push_back(this);
}

CollObj::~CollObj()
{
    get_current_coll_world()->DestroyBody(collision_body);

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
    
    pos computed = global_position;

    collision_body->SetTransform((computed / B2_SCALE),global_position.compute_angle());
}

void CollObj::collision_process(double delta)
{
    b2Vec2 body_b2pos = collision_body->GetTransform().p;
    pos body_pos(body_b2pos.x * B2_SCALE, body_b2pos.y * B2_SCALE);
    pos offset = {0, 0};
    
    if (global_position.parent != nullptr)
    {        
        offset = *global_position.parent;
    }
    
    position = body_pos - offset;

    angle = collision_body->GetAngle();
}
