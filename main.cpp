#include <iostream>
#include <vector>

#include "game.hpp"
#include "drawobj.hpp"
#include "blendobj.hpp"
#include "collobj.hpp"

#include "joint.hpp"

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

class Mouse : public CollObj
{
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

        name = "Mouse";
    }

    void process(double delta)
    {
        float x,y;

        SDL_GetMouseState(&x,&y);

        if (x != NULL && y != NULL) b2Body_SetLinearVelocity(collision_body, b2Vec2{(x - (float)position.x) * 60.0f, (y - (float)position.y) * 60.0f});
    }
};

int main()
{
    b2Init();

    b2WorldDef world_def = WorldDef(pos(0,100));

    b2WorldId coll_world = b2CreateWorld(&world_def);
    set_current_coll_world(coll_world);

    game* gameplay = new game;
    set_current_game(gameplay);

    gameplay->coll_world = coll_world;

    BlendObj* drawer = new BlendObj;
    drawer->set_position({0,0});

    BLImage img(480, 480, BLEND_FORMAT);

    BLContext ctx(img);
    ctx.clearAll();

    BLGradient linear(
    BLLinearGradientValues(0, 0, 480, 480));
    linear.addStop(0.0, BLRgba32(0x22FF00FF));
    linear.addStop(1.0, BLRgba32(0xFF00FF00));

    ctx.fillRoundRect(
    BLRoundRect(195, 195, 270, 270, 25), linear);

    drawer->set_image(img);
    drawer->set_size({1000,500});

    StaticObj* obj = new StaticObj;
    gameplay->root->add_child(obj);

    Mouse* mse = new Mouse;
    DrawObj* spri = new DrawObj;

    mse->set_position({500,500});

    SDL_Surface* surf = SDL_LoadBMP("sample2.bmp");
    SDL_Texture* bmp = SDL_CreateTextureFromSurface(gameplay->game_window->renderer, surf);
    SDL_DestroySurface(surf);

    spri->set_sprite(bmp, false);

    mse->add_child(spri);

    gameplay->root->add_child(mse);

    pos center(250,250);

    for (int x = 0; x < 53; ++x)
    {
        DynamObj* ball = new DynamObj;
        DrawObj* spri = new DrawObj;
    
        ball->set_position({500,500});
    
        spri->set_sprite(bmp, false);
    
        ball->set_position(pos(250,10));
    
        ball->add_child(spri);
    
        gameplay->root->add_child(ball);
    }

    std::vector<distance_j> joints;

    CollObj* past = mse;

    b2DistanceJointDef j_def = b2DefaultDistanceJointDef();
    j_def.localAnchorA = pos(0, 0);
    j_def.localAnchorB = pos(0,0);
    j_def.length = size + 1;

    const int total = 50;
    for (int x = 0; x < total; ++x)
    {
        DynamObj* object = new DynamObj;
        DrawObj* sprite = new DrawObj;
    
        sprite->set_sprite(bmp, false);

        object->set_position(center + pos(x * size * 3, 0));
    
        object->add_child(sprite);
    
        gameplay->root->add_child(object);

        distance_j joint = distance_j(past, object, j_def);
        joints.push_back(joint);

        past = object;
    }

    b2DistanceJointDef j_def2 = b2DefaultDistanceJointDef();
    j_def.localAnchorA = pos(0, 0);
    j_def.localAnchorB = pos(100,250);
    j_def.length = size * 2;
    j_def.collideConnected = true;
    distance_j joint = distance_j(past, obj, j_def);
    joints.push_back(joint);

    std::cout << joints.size() << std::endl;
    int count = 0;

    for (_joint j : joints)
    {
        if (j)
        {
            count++;
        }
    }
    std::cout << count << std::endl;

    gameplay->start();

    delete gameplay;

    return 0;
}