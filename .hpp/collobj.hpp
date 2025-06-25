
#pragma once

#include "factory.hpp"
#include "object.hpp"

#include "hull.hpp"
#include <box2d/math_functions.h>

EXTERNAL_VAR_CONSTRUCTOR(b2Vec2)
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

EXTERNAL_VAR_CONSTRUCTOR(b2Rot)
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
class CollObj : public Object
{
ASSIGN_CONSTRUCTOR(CollObj)

protected:
    b2BodyId collision_body;
    b2BodyDef collision_def;

    bool body_ownership = true;

    std::string hull_path = "";

public:
    CollObj();
    
    ~CollObj();

    void _process(double delta);

    virtual void collision_process();

    void set_collision_body(b2BodyId new_body, bool ownership);

    b2BodyId get_collision_body();

    void set_collision_def(b2BodyDef new_def); // Does NOTHING to the body

    b2BodyDef get_collision_def();

    void set_body_ownership(bool ownership);

    bool owns_body();

    void delete_body();

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
    void load(Loader* ar)
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
        collision_def.fixedRotation = ar->load_data<bool>();
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

    void save(Saver* ar) const
    {
        Object::save(ar);
        
        ar->save_complex(hull_path);

        ar->save_data(collision_def.allowFastRotation);
        ar->save_data(collision_def.angularDamping);
        ar->save_data(collision_def.enableSleep);
        ar->save_data(collision_def.fixedRotation);
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
