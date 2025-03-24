
#pragma once

#include "object.hpp"

#include <box2d/box2d.h>

const float B2_SCALE = 50.0f; // Difference between b2 positions and my positions
const float B2_MAX_MOVEMENT = 5000.0f;

class CollObj : public Object
{
protected:
    b2BodyId collision_body;
    b2BodyDef collision_def;

public:
    CollObj();
    
    ~CollObj();

    void process(double delta);

    virtual void collision_process();

    static void CollisionBegin(b2ShapeId a, b2ShapeId b);

    static void CollisionEnd(b2ShapeId a, b2ShapeId b);

    static void SensorBegin(b2ShapeId sensor, b2ShapeId visitor);

    static void SensorEnd(b2ShapeId sensor, b2ShapeId visitor);

    virtual void collide_begin(CollObj* other);

    virtual void collide_end(CollObj* other);

    virtual void sensor_begin(CollObj* other);

    virtual void sensor_end(CollObj* other);
};
