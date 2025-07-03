
// USER DEFINED STUFF ISH
#include "factory.hpp"
static const int HUH = 1000000;
static const int width = 500;

#include "collobj.hpp"

class StaticObj : public CollObj
{
ASSIGN_CONSTRUCTOR(StaticObj)

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
ASSIGN_CONSTRUCTOR(DynamObj)

private:
    double counter = 60;

public:
    DynamObj() = default;

    void onload()
    {
        collision_def.type = b2_dynamicBody;
        collision_def.enableSleep = false;

        collision_body = b2CreateBody(get_current_coll_world(), &collision_def);

        b2Polygon box = b2MakeBox(size, size);
        
        b2ShapeDef fixtureDef = b2DefaultShapeDef();
        fixtureDef.density = 0.0001;
        fixtureDef.friction = 0;
        fixtureDef.restitution = 1;
        fixtureDef.filter.categoryBits = 2;
        fixtureDef.filter.maskBits = 1;

        b2CreatePolygonShape(collision_body, &fixtureDef, &box);
    }

    void process(double delta)
    {
        counter -= delta;

        if (counter <= 0) start_delete();
    }
};

class Mouse : public CollObj
{
ASSIGN_CONSTRUCTOR(Mouse)

public:
    Mouse()
    {
        collision_def.type = b2_kinematicBody;
        collision_def.isBullet = true;

        collision_body = b2CreateBody(get_current_coll_world(), &collision_def);

        b2Circle circ;
        circ.radius = size;

        b2ShapeDef def = b2DefaultShapeDef();

        b2CreateCircleShape(collision_body, &def, &circ);
    }

    void process(double)
    {
        float x,y;

        SDL_GetMouseState(&x,&y);

        if (x != 0 && y != 0) b2Body_SetLinearVelocity(collision_body, b2Vec2{(x - (float)position.x) * 60.0f, (y - (float)position.y) * 60.0f});
    }
};