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
    b2Init();

    b2WorldDef world_def = WorldDef(pos(-100,0));

    b2WorldId coll_world = b2CreateWorld(&world_def);
    set_current_coll_world(coll_world);

    game* gameplay = new game;
    set_current_game(gameplay);

    gameplay->coll_world = coll_world;

    BlendObj* drawer2 = new BlendObj;
    drawer2->set_position({100,100});

    BLImage img2(480, 480, BL_FORMAT_PRGB32);
    BLContext ctx2(img2);
  
    ctx2.clearAll();
  
    // Coordinates can be specified now or changed
    // later via BLGradient accessors.
    BLGradient linear2(BLLinearGradientValues(0, 0, 480, 480));
  
    // Color stops can be added in any order.
    linear2.addStop(0.0, BLRgba32(0xFF32aaFF));
    linear2.addStop(0.5, BLRgba32(0x225FAFDF));
    linear2.addStop(1.0, BLRgba32(0xFF2F5FDF));
  
    // `setFillStyle()` can be used for both colors
    // and styles. Alternatively, a color or style
    // can be passed explicitly to a render function.
    ctx2.setFillStyle(linear2);
  
    // Rounded rect will be filled with the linear
    // gradient.
    ctx2.fillRoundRect(40.0, 40.0, 400.0, 400.0, 45.5);
    ctx2.end();

    drawer2->set_image(img2);

    StaticObj* obj = new StaticObj;
    gameplay->root->add_child(obj);

    Mouse* mse = new Mouse;
    DrawObj* spri = new DrawObj;

    SDL_Surface* surf = SDL_LoadBMP("sample2.bmp");
    SDL_Texture* bmp = SDL_CreateTextureFromSurface(gameplay->game_window->renderer, surf);
    SDL_DestroySurface(surf);

    spri->set_sprite(bmp, false);

    mse->set_position(pos(250,250));

    mse->add_child(spri);

    gameplay->root->add_child(mse);

    pos center(250,250);

    std::vector<distance_j*> joints;

    CollObj* past = nullptr;

    b2DistanceJointDef j_def = b2DefaultDistanceJointDef();
    j_def.localAnchorA = pos(size, 0);
    j_def.localAnchorB = pos(-size,0);
    j_def.length = 1;

    const int total = 80;
    for (int x = 0; x < total; ++x)
    {
        DynamObj* obj = new DynamObj;
        DrawObj* sprite = new DrawObj;
    
        sprite->set_sprite(bmp, false);

        obj->set_position(center + pos(x * size * 3, 0));
    
        obj->add_child(sprite);
    
        gameplay->root->add_child(obj);

        if (past != nullptr)
        {
            joints.push_back(new distance_j(past, obj, j_def));
        }

        past = obj;
    }

    gameplay->start();

    delete gameplay;

    return 0;
}