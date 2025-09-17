
#pragma once

#include "box2d/box2d.h"

#include ".hpp/object.hpp"

#include ".hpp/hull.hpp"
#include "box2d/id.h"

EXTERNAL_VAR_CONSTRUCTOR(b2Vec2);
OVERRIDE_LOAD(b2Vec2)
{
    b2Vec2 data;
    data.x = load_data<float>();
    data.y = load_data<float>();

    return data;
}
OVERRIDE_SAVE(b2Vec2)(b2Vec2 data)
{
    save_data(data.x);
    save_data(data.y);
}

EXTERNAL_VAR_CONSTRUCTOR(b2Rot);
OVERRIDE_LOAD(b2Rot)
{
    b2Rot data;
    data.c = load_data<float>();
    data.s = load_data<float>();

    return data;
}
OVERRIDE_SAVE(b2Rot)(b2Rot data)
{
    save_data(data.c);
    save_data(data.s);
}

// TODO set velocity functions
// CollObjs have a global position and will not follow an object that they are a child of. The position will still be relative to the object (with a slight determinancy issue)
// Any changed b2 values will not be up to date until after the collision_process() call
class CollObj : public Object
{
ASSIGN_CONSTRUCTOR(CollObj);

protected:
    b2BodyId collision_body;
    b2BodyDef collision_def;

    bool body_ownership = true;

    std::string hull_path = "";

    pos lin_velocity = {0,0};
    rad rot_velocity = 0;

public:
    CollObj();
    
    ~CollObj();

    // Update all b2 values to be up to date with this
    void set_collision_info()
    {
        b2Body_SetTransform(collision_body,global_position.compute(),global_position.compute_angle());

        b2Body_SetLinearVelocity(collision_body, lin_velocity);
        b2Body_SetAngularVelocity(collision_body, rot_velocity);
    }

    // Updates all values to be up to date with b2
    void update_collision_info()
    {
        if (b2Body_IsValid(collision_body))
        {
            pos body_pos = b2Body_GetPosition(collision_body);
            
            if (global_position.parent != nullptr)
            {        
                const pos offset = global_position.parent->compute();
                rad offset_rad = global_position.parent->compute_angle();

                position = (body_pos - offset).rotated(-offset_rad);

                angle = rad(b2Body_GetRotation(collision_body)) - offset_rad;
            }

            else
            {
                position = body_pos;

                angle = rad(b2Body_GetRotation(collision_body));
            }

            lin_velocity = b2Body_GetLinearVelocity(collision_body);
            rot_velocity = b2Body_GetAngularVelocity(collision_body);
        }
    }

    virtual void collision_process();

    void set_collision_body(b2BodyId new_body, bool ownership);

    b2BodyId get_collision_body();

    void set_collision_def(b2BodyDef new_def); // Does NOTHING to the body

    b2BodyDef get_collision_def();

    void set_body_ownership(bool ownership);

    bool owns_body();

    void delete_body();

    void set_lin_velocity(pos new_velocity)
    {
        lin_velocity = new_velocity;
    }

    pos get_lin_velocity() const
    {
        return lin_velocity;
    }

    void set_rot_velocity(rad new_velocity)
    {
        rot_velocity = new_velocity;
    }

    rad get_rot_velocity() const
    {
        return rot_velocity;
    }

    // ---------- COLLISION CALLBACK ISH STUFF
    static void CollisionBegin(b2ShapeId a, b2ShapeId b);

    static void CollisionEnd(b2ShapeId a, b2ShapeId b);

    static void SensorBegin(b2ShapeId sensor, b2ShapeId visitor);

    static void SensorEnd(b2ShapeId sensor, b2ShapeId visitor);

    virtual void collide_begin(CollObj* other);

    virtual void collide_end(CollObj* other);

    virtual void sensor_begin(CollObj* other);

    virtual void sensor_end(CollObj* other);

    // TODO decide if this should create the physics body and if it should store a physics body
    void load(Loader* ar) override
    {
        Object::load(ar);
        
        hull_path = ar->load_complex<std::string>();

        if (hull_path.size() > 0)
        {
            hull obj_hull;
            obj_hull.load(hull_path);
        }

        collision_def.allowFastRotation = ar->load_data<bool>();
        collision_def.angularDamping = ar->load_data<float>();
        collision_def.enableSleep = ar->load_data<bool>();
        collision_def.gravityScale = ar->load_data<float>();
        collision_def.isAwake = ar->load_data<bool>();
        collision_def.isBullet = ar->load_data<bool>();
        collision_def.isEnabled = ar->load_data<bool>();
        collision_def.linearDamping = ar->load_data<float>();
                
        // Do not save nor load collision_def.name

        collision_def.position = ar->load_complex<b2Vec2>();
        collision_def.rotation = ar->load_complex<b2Rot>();
        collision_def.linearVelocity = ar->load_complex<b2Vec2>();
        collision_def.angularVelocity = ar->load_data<float>();
        collision_def.sleepThreshold = ar->load_data<float>();
        collision_def.type = ar->load_enum<b2BodyType>();
    }

    void save(Saver* ar) const override
    {
        Object::save(ar);
        
        ar->save_complex(hull_path);

        ar->save_data(collision_def.allowFastRotation);
        ar->save_data(collision_def.angularDamping);
        ar->save_data(collision_def.enableSleep);
        ar->save_data(collision_def.gravityScale);
        ar->save_data(collision_def.isAwake);
        ar->save_data(collision_def.isBullet);
        ar->save_data(collision_def.isEnabled);
        ar->save_data(collision_def.linearDamping);

        // Do not save nor load collision_def.name

        ar->save_complex(collision_def.position);
        ar->save_complex(collision_def.rotation);
        
        if (b2Body_IsValid(collision_body))
        {
            ar->save_complex(b2Body_GetLinearVelocity(collision_body));
            ar->save_data(b2Body_GetAngularVelocity(collision_body));
        }
        
        else
        {
            ar->save_complex(collision_def.linearVelocity);
            ar->save_data(collision_def.angularVelocity);
        }
        
        ar->save_data(collision_def.sleepThreshold);
        ar->save_enum(collision_def.type);
    }
};

CollObj* get_shape_owner(b2ShapeId shape);