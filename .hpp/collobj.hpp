
#pragma once

#include "object.hpp"

#include "b2.h"

class CollObj : public Object
{
protected:
    b2BodyId collision_body;
    b2BodyDef collision_def;

    bool body_ownership = true;

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
};
