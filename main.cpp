#include <iostream>
#include <vector>

#include "game.hpp"
#include "drawobj.hpp"
#include "blendobj.hpp"
#include "collobj.hpp"

// USER DEFINED STUFF ISH
#define HUH 10000

class StaticObj : public CollObj
{
public:
    StaticObj()
    {
        collision_def.type = b2_staticBody;
        collision_body = b2CreateBody(get_current_coll_world(), &collision_def);

        b2Polygon box1 = b2MakeOffsetBox(HUH,HUH, pos(-HUH + 100, HUH), b2MakeRot(0));

        b2Polygon box2 = b2MakeOffsetBox(HUH,HUH, pos(HUH, -HUH), b2MakeRot(0));

        b2Polygon box3 = b2MakeOffsetBox(HUH,HUH, pos(HUH, 500 + HUH), b2MakeRot(0));

        b2ShapeDef shapedef = b2DefaultShapeDef();
        
        b2CreatePolygonShape(collision_body, &shapedef, &box1);
        b2CreatePolygonShape(collision_body, &shapedef, &box2);
        b2CreatePolygonShape(collision_body, &shapedef, &box3);
    }
};

float size = 6.0f;

class DynamObj : public CollObj
{
public:
    DynamObj()
    {
        collision_def.type = b2_dynamicBody;

        collision_body = b2CreateBody(get_current_coll_world(), &collision_def);

        b2Polygon box = b2MakeBox(size, size);
        
        b2ShapeDef fixtureDef = b2DefaultShapeDef();
        fixtureDef.density = 1;
        fixtureDef.friction = 1;
        fixtureDef.restitution = 0.4;

        b2CreatePolygonShape(collision_body, &fixtureDef, &box);
        
        name = "Box";
    }
};

float radi = 20;
class Mouse : public CollObj
{
public:
    Mouse()
    {
        collision_def.type = b2_kinematicBody;
        collision_def.isBullet = true;

        collision_body = b2CreateBody(get_current_coll_world(), &collision_def);

        b2Circle circ;
        circ.radius = radi;

        b2ShapeDef def = b2DefaultShapeDef();

        b2CreateCircleShape(collision_body, &def, &circ);

        name = "Mouse";
    }

    void process(double delta)
    {
        float x,y;

        SDL_GetMouseState(&x,&y);

        b2Body_SetLinearVelocity(collision_body, b2Vec2{(x - (float)position.x) * 60.0f, (y - (float)position.y) * 60.0f});
    }
};

#include "joint.hpp"

int main()
{
    game* gameplay = new game;
    set_current_game(gameplay);
    gameplay->physics = false;

    BlendObj* drawer = new BlendObj;
    drawer->set_position({100,100});

    BLImage img(480, 480, BLEND_FORMAT);

    // Attach a rendering context into `img`.
    BLContext ctx(img);
    ctx.clearAll();

    // First shape filled with a radial gradient.
    // By default, SRC_OVER composition is used.
    BLGradient radial(
    BLRadialGradientValues(180, 180, 180, 180, 180));
    radial.addStop(0.0, BLRgba32(0xFFFFFFFF));
    radial.addStop(1.0, BLRgba32(0xFFFF6F3F));
    ctx.fillCircle(180, 180, 160, radial);
  
    // Second shape filled with a linear gradient.
    BLGradient linear(
    BLLinearGradientValues(195, 195, 470, 470));
    linear.addStop(0.0, BLRgba32(0xFFFFFFFF));
    linear.addStop(1.0, BLRgba32(0xFF3F9FFF));
  
    // Use 'setCompOp()' to change a composition operator.
    ctx.setCompOp(BL_COMP_OP_DIFFERENCE);
    ctx.fillRoundRect(
      BLRoundRect(195, 195, 270, 270, 25), linear);

    drawer->set_image(img);
    drawer->set_size({500,200});

    gameplay->start();

    delete gameplay;

    return 0;
}