
#include "game.hpp"
#include "drawobj.hpp"
#include "blendobj.hpp"
#include "collobj.hpp"
#include "joint.hpp"
#include "editorstuff.hpp"

#include <iostream>
#include <fstream>


// USER DEFINED STUFF ISH
#define HUH 10000

class StaticObj : public CollObj
{
REGISTER_OBJECT(StaticObj)

ARCHIVE_INHERIT(CollObj)

public:
    StaticObj()
    {
        collision_def.type = b2_staticBody;
        collision_body = b2CreateBody(get_current_coll_world(), &collision_def);

        b2Polygon box3 = b2MakeOffsetBox(HUH,HUH, pos(HUH, 500 + HUH), b2MakeRot(0));

        b2ShapeDef shapedef = b2DefaultShapeDef();

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
    }

    void process(double)
    {
        float x,y;

        SDL_GetMouseState(&x,&y);

        if (x != 0 && y != 0) b2Body_SetLinearVelocity(collision_body, b2Vec2{(x - (float)position.x) * 60.0f, (y - (float)position.y) * 60.0f});
    }
};

int main()
{
    game gameplay;
    set_current_game(&gameplay);

    b2Init();
    
    b2WorldDef def = WorldDef(pos(0,30));
    b2WorldId world = b2CreateWorld(&def);

    set_current_coll_world(world);
    gameplay.coll_world = world;

    StaticObj* obj = new StaticObj;

    Process* root_obj = new Process;

    SDL_Surface* surf = SDL_LoadBMP("sample2.bmp");

    SDL_Texture* text = SDL_CreateTextureFromSurface(gameplay.game_window->renderer,surf);
    SDL_DestroySurface(surf);

    for (int x = 0; x < 100; x++)
    {
        DynamObj* dyn = new DynamObj;

        dyn->set_position({400,-10.0 * x});

        DrawObj* drawer = new DrawObj;
        dyn->add_child(drawer);

        drawer->set_sprite(text, false);

        root_obj->add_child(dyn);
    }

    gameplay.root->add_child(obj);

    gameplay.root->add_child(root_obj);

    gameplay.start();

    {
        std::ofstream fil("of.json");
        cereal::JSONOutputArchive os(fil);

        ProcessFactory::saveToArchive(os, root_obj);
    }

    return 0;
}