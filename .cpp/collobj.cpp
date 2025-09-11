
#include ".hpp/collobj.hpp"

#include ".hpp/b2.h"
#include ".hpp/game.hpp"

CollObj::CollObj()
{
    // TODO init() instead of this
    get_current_game()->add_to_collisions(this);

    collision_def = BodyDef(this);
}

CollObj::~CollObj()
{
    delete_body();

    get_current_game()->remove_from_collisions(this);
}

void CollObj::collision_process()
{
    return;
}

void CollObj::set_collision_body(b2BodyId new_body, bool ownership)
{
    delete_body();

    body_ownership = ownership;
    collision_body = new_body;
}

b2BodyId CollObj::get_collision_body()
{
    return collision_body;
}

void CollObj::set_collision_def(b2BodyDef new_def) // Does NOTHING to the body
{
    collision_def = new_def;
}

b2BodyDef CollObj::get_collision_def()
{
    return collision_def;
}

void CollObj::set_body_ownership(bool ownership)
{
    body_ownership = ownership;
}

bool CollObj::owns_body()
{
    return body_ownership;
}

void CollObj::delete_body()
{
    if (body_ownership && b2Body_IsValid(collision_body))
    {
        b2DestroyBody(collision_body);
    }
}

// Collision Callbacks

CollObj* get_shape_owner(b2ShapeId shape)
{
    return (CollObj*)b2Body_GetUserData(b2Shape_GetBody(shape));
}

void CollObj::CollisionBegin(b2ShapeId a, b2ShapeId b)
{
    if (b2Shape_IsValid(a) && b2Shape_IsValid(b))
    {
        CollObj* a_obj = get_shape_owner(a);
        CollObj* b_obj = get_shape_owner(b);

        a_obj->collide_begin(b_obj);
        b_obj->collide_begin(a_obj);
    }
}

void CollObj::CollisionEnd(b2ShapeId a, b2ShapeId b)
{
    if (b2Shape_IsValid(a) && b2Shape_IsValid(b))
    {
        CollObj* a_obj = get_shape_owner(a);
        CollObj* b_obj = get_shape_owner(b);

        a_obj->collide_end(b_obj);
        b_obj->collide_end(a_obj);
    }
}

void CollObj::SensorBegin(b2ShapeId sensor, b2ShapeId visitor)
{
    if (b2Shape_IsValid(sensor) && b2Shape_IsValid(visitor))
    {
        CollObj* sensor_obj = get_shape_owner(sensor);
        CollObj* visitor_obj = get_shape_owner(visitor);

        sensor_obj->sensor_begin(visitor_obj);
    }
}

void CollObj::SensorEnd(b2ShapeId sensor, b2ShapeId visitor)
{
    if (b2Shape_IsValid(sensor) && b2Shape_IsValid(visitor))
    {
        CollObj* sensor_obj = get_shape_owner(sensor);
        CollObj* visitor_obj = get_shape_owner(visitor);

        sensor_obj->sensor_end(visitor_obj);
    }
}

void CollObj::collide_begin(CollObj*)
{
    return;
}

void CollObj::collide_end(CollObj*)
{
    return;
}

void CollObj::sensor_begin(CollObj*)
{
    return;
}

void CollObj::sensor_end(CollObj*)
{
    return;
}