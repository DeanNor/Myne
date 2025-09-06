
// USER DEFINED STUFF ISH
#include ".hpp/factory.hpp"

#include ".hpp/game.hpp"
#include "box2d/types.h"

static const int HUH = 1000000;
static const int width = 500;

#include ".hpp/collobj.hpp"

class StaticObj : public CollObj
{
ASSIGN_CONSTRUCTOR(StaticObj);

public:
    StaticObj()
    {
        collision_def.type = b2_staticBody;
        collision_body = b2CreateBody(get_current_coll_world(), &collision_def);

        b2Polygon box1 = b2MakeOffsetBox(HUH,HUH, pos(HUH + 2*width, HUH), b2MakeRot(0));

        b2Polygon box2 = b2MakeOffsetBox(HUH,HUH, pos(-HUH,HUH), b2MakeRot(0));

        b2Polygon box3 = b2MakeOffsetBox(HUH,HUH, pos(HUH - 1000,  width + HUH), 0_r);

        //b2Polygon box4 = b2MakeOffsetBox(HUH,HUH, pos(HUH, -HUH), b2MakeRot(0));

        b2ShapeDef shapedef = b2DefaultShapeDef();

        b2CreatePolygonShape(collision_body, &shapedef, &box1);
        b2CreatePolygonShape(collision_body, &shapedef, &box2);
        b2CreatePolygonShape(collision_body, &shapedef, &box3);
        //b2CreatePolygonShape(collision_body, &shapedef, &box4);
    }
};

static float size = 6.0f;

class DynamObj : public CollObj
{
ASSIGN_CONSTRUCTOR(DynamObj);

public:
    DynamObj()
    {
        collision_def.type = b2_dynamicBody;
        collision_def.enableSleep = false;

        collision_body = b2CreateBody(get_current_coll_world(), &collision_def);

        b2Polygon box = b2MakeBox(size, size);
        
        b2ShapeDef fixtureDef = b2DefaultShapeDef();
        fixtureDef.density = 0.0001;
        fixtureDef.material.friction = 0;
        fixtureDef.material.restitution = 1;
        fixtureDef.filter.categoryBits = 2;
        fixtureDef.filter.maskBits = 1;

        b2CreatePolygonShape(collision_body, &fixtureDef, &box);
    }
};

class Mouse : public CollObj
{
ASSIGN_CONSTRUCTOR(Mouse);

public:
    Mouse()
    {
        collision_def.type = b2_dynamicBody;
        collision_def.isBullet = false;
        collision_def.motionLocks.angularZ = true;

        collision_body = b2CreateBody(get_current_coll_world(), &collision_def);

        b2Circle circ;
        circ.radius = size;

        b2ShapeDef def = b2DefaultShapeDef();

        b2CreateCircleShape(collision_body, &def, &circ);
    }

    void collision_process() override
    {
        CollObj::collision_process();

        pos mouse_pos = get_current_game()->mouse.position;

        if (mouse_pos.x != 0 && mouse_pos.y != 0) b2Body_SetLinearVelocity(collision_body, b2Vec2{(float)(mouse_pos.x - position.x) * 3.f, (float)(mouse_pos.y - position.y) * 3.f});
    }
};