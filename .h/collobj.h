
#pragma once

#include "object.h"

#include <box2d/box2d.h>

class CollObj : public Object
{
protected:
    b2Shape* collision_shape = nullptr;
    b2Body* collision_body = nullptr;
    b2BodyDef collision_def;

public:
    CollObj();

    void process(double delta);

    virtual void collision_process();
};
