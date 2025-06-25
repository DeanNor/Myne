
#include "collobj.hpp"

CollObj::CollObj()
{
    get_current_game()->collisions.push_back(this);

    collision_def = BodyDef(this);
}

CollObj::~CollObj()
{
    delete_body();

    get_current_game()->remove_from_collisions(this);
}

void CollObj::_process(double delta)
{
    Object::_process(delta);

    b2Body_SetTransform(collision_body,global_position.compute(),global_position.compute_angle());
}

void CollObj::collision_process()
{
    if (b2Body_IsValid(collision_body))
    {
        pos body_pos = b2Body_GetPosition(collision_body);
        
        if (global_position.parent != nullptr)
        {        
            pos offset = global_position.parent->compute();
            rad offset_rad = global_position.parent->compute_angle();

            position = (body_pos - offset).rotated(-offset_rad);

            angle = rad(b2Body_GetRotation(collision_body)) - offset_rad;
        }

        else
        {
            position = body_pos;

            angle = rad(b2Body_GetRotation(collision_body));
        }
    }
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

CollObj* obj_from_shape(b2ShapeId shape)
{
    return (CollObj*)b2Body_GetUserData(b2Shape_GetBody(shape));
}

void CollObj::CollisionBegin(b2ShapeId a, b2ShapeId b)
{
    if (b2Shape_IsValid(a) && b2Shape_IsValid(b))
    {
        CollObj* a_obj = obj_from_shape(a);
        CollObj* b_obj = obj_from_shape(b);

        a_obj->collide_begin(b_obj);
        b_obj->collide_begin(a_obj);
    }
}

void CollObj::CollisionEnd(b2ShapeId a, b2ShapeId b)
{
    if (b2Shape_IsValid(a) && b2Shape_IsValid(b))
    {
        CollObj* a_obj = obj_from_shape(a);
        CollObj* b_obj = obj_from_shape(b);

        a_obj->collide_end(b_obj);
        b_obj->collide_end(a_obj);
    }
}

void CollObj::SensorBegin(b2ShapeId sensor, b2ShapeId visitor)
{
    if (b2Shape_IsValid(sensor) && b2Shape_IsValid(visitor))
    {
        CollObj* sensor_obj = obj_from_shape(sensor);
        CollObj* visitor_obj = obj_from_shape(visitor);

        sensor_obj->sensor_begin(visitor_obj);
    }
}

void CollObj::SensorEnd(b2ShapeId sensor, b2ShapeId visitor)
{
    if (b2Shape_IsValid(sensor) && b2Shape_IsValid(visitor))
    {
        CollObj* sensor_obj = obj_from_shape(sensor);
        CollObj* visitor_obj = obj_from_shape(visitor);

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