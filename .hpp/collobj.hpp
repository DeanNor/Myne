
#pragma once

#include "object.hpp"

#include "hull.hpp"

// TODO set velocity
class CollObj : public Object
{
REGISTER_OBJECT(CollObj)

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
    template <class Archive>
    void load(Archive& ar)
    {
        ar(cereal::base_class<Object>(this));
        
        ar(hull_path);

        if (hull_path.size() > 0)
        {
            hull obj_hull;
            obj_hull.load(hull_path);
        }

        std::string def_name;
        pos def_position, def_vel;
        rad def_rotation, def_rot_vel;

        ar(collision_def.allowFastRotation, collision_def.angularDamping, collision_def.enableSleep, collision_def.fixedRotation, collision_def.gravityScale, collision_def.isAwake, collision_def.isBullet, collision_def.isEnabled, collision_def.linearDamping, def_name, def_position, def_rotation, def_vel, def_rot_vel, collision_def.sleepThreshold, collision_def.type);

        // Non-auto-typed stuff
        collision_def.name = def_name.c_str();
        collision_def.position = def_position;
        collision_def.rotation = def_rotation;

        collision_def.linearVelocity = def_vel;
        collision_def.angularVelocity = def_rot_vel;
    }

    template <class Archive>
    void save(Archive& ar) const
    {
        ar(cereal::base_class<Object>(this));
        
        ar(hull_path);

        ar(collision_def.allowFastRotation, collision_def.angularDamping, collision_def.enableSleep, collision_def.fixedRotation, collision_def.gravityScale, collision_def.isAwake, collision_def.isBullet, collision_def.isEnabled, collision_def.linearDamping);
        
        if (collision_def.name != nullptr && strlen(collision_def.name) != 0)
        {
            ar(std::string(collision_def.name));
        }
        else ar(std::string());

        ar(pos(collision_def.position), rad(collision_def.rotation));
        
        if (b2Body_IsValid(collision_body))
        {
            ar(pos(b2Body_GetLinearVelocity(collision_body)), rad(b2Body_GetAngularVelocity(collision_body)));
        }
        
        else
        {
            ar(pos(collision_def.linearVelocity), rad(collision_def.angularVelocity));
        }
        
        ar(collision_def.sleepThreshold, collision_def.type);
    }
};
