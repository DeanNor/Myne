#pragma once

#include <box2d/box2d.h>
#include <box2d/types.h>
#include <box2d/math_functions.h>

static float B2_SCALE = 100.0f; // Scale between b2 positions and object positions
static float B2_MAX_MOVEMENT = 500.0f;

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
