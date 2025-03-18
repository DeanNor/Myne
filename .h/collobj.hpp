
#pragma once

#include "object.hpp"

#include <box2d/box2d.h>

#define B2_SCALE 100.f // Difference between b2 positions and my positions

class CollObj : public Object
{
protected:
    b2Body* collision_body = nullptr;

public:
    CollObj();
    
    ~CollObj();

    void process(double delta);

    virtual void collision_process(double delta);
};
