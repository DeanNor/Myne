
#pragma once

#include "object.hpp"

#include <box2d/box2d.h>

const float B2_SCALE = 1.0f / 5.f; // Difference between b2 positions and my positions

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

    virtual void collide_with(CollObj* other);
};
