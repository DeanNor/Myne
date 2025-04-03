#pragma once

#include <box2d/box2d.h>

const float B2_SCALE = 6.0f; // Difference between b2 positions and my positions
const float B2_MAX_MOVEMENT = 50000.0f;

inline void b2Init()
{
    b2SetLengthUnitsPerMeter(B2_SCALE);
}

inline b2WorldDef WorldDef(b2Vec2 gravity)
{
    b2WorldDef def = b2DefaultWorldDef();

    def.gravity = gravity;

    def.maximumLinearSpeed = B2_MAX_MOVEMENT * B2_SCALE;

    return def;
}

inline b2BodyDef BodyDef(void* owner)
{
    b2BodyDef def = b2DefaultBodyDef();

    def.userData = owner;

    return def;
}
