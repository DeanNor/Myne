#include <iostream>
#include <vector>

#include "game.hpp"
#include "drawobj.hpp"
#include "collobj.hpp"

// Whenever I want to convert this to SDL, I will
/*pos to_tileset(pos value, pos tile_size);

class Tileset : public DrawObj
{
protected:
    std::vector<pos> tiles;

    ALLEGRO_BITMAP* tilemap = nullptr;

    pos tile_size = {10, 10};

    ALLEGRO_COLOR tilecol = al_map_rgb_f(1,1,1);

    bool redraw = false;

public:
    Tileset()
    {
        tilemap = al_create_bitmap(500,500);
    }

    void draw()
    {
        if (redraw)
        {
            update_map();
        }

        al_draw_bitmap(tilemap,position.x,position.y,0);
    }

    void update_map()
    {
        ALLEGRO_BITMAP* main_bitmap = al_get_target_bitmap();
        
        al_set_target_bitmap(tilemap);

        for (pos tile : tiles)
        {
            al_draw_bitmap(sprite,tile.x,tile.y,0);
        }

        al_set_target_bitmap(main_bitmap);
    }

    bool tile_used(pos tile_position)
    {
        tile_position = to_tileset(tile_position, tile_size);

        for (pos tile : tiles)
        {
            if (tile_position == tile)
            {
                return true;
            }
        }

        return false;
    }
};

pos to_tileset(pos value, pos tile_size)
{
    pos tile_number = value / tile_size;
    tile_number = tile_number.round();
    return tile_number * tile_size;
}
*/


// USER DEFINED STUFF ISH
#define HUH 1000

class StaticObj : public CollObj
{
public:
    StaticObj()
    {
        collision_def.type = b2_staticBody;
        collision_body = b2CreateBody(get_current_coll_world(), &collision_def);

        b2Polygon box1 = b2MakeOffsetBox(HUH,HUH, pos(-HUH, HUH), b2MakeRot(0));

        b2Polygon box2 = b2MakeOffsetBox(HUH,HUH, pos(HUH, -HUH), b2MakeRot(0));

        b2Polygon box3 = b2MakeOffsetBox(HUH,HUH, pos(HUH, 500 + HUH), b2MakeRot(0));

        b2ShapeDef shapedef = b2DefaultShapeDef();
        
        b2CreatePolygonShape(collision_body, &shapedef, &box1);
        b2CreatePolygonShape(collision_body, &shapedef, &box2);
        b2CreatePolygonShape(collision_body, &shapedef, &box3);
    }
};

#define size 6

float diam = 100;
class Mouse : public CollObj
{
public:
    Mouse()
    {
        collision_def.type = b2_kinematicBody;

        collision_body = b2CreateBody(get_current_coll_world(), &collision_def);

        b2Circle box;
        box.center = pos(diam / 2, diam / 2);
        box.radius = diam;
        
        b2ShapeDef fixtureDef = b2DefaultShapeDef();
        fixtureDef.enableContactEvents = true;
        fixtureDef.density = 1;
        fixtureDef.friction = 1;
        fixtureDef.restitution = 0.5f;
        fixtureDef.isSensor = true;

        b2CreateCircleShape(collision_body, &fixtureDef, &box);

        name = "Mouse";
    }

    void process(double delta)
    {
        CollObj::process(delta);

        float x,y;

        SDL_GetMouseState(&x,&y);

        pos glob = global_position;

        b2Body_SetLinearVelocity(collision_body, (pos(x,y) - glob) * 60.0);
    }

    void sensor_begin(CollObj* other)
    {
        other->start_delete();
    }
};

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
        fixtureDef.restitution = 0.5f;

        b2CreatePolygonShape(collision_body, &fixtureDef, &box);
        
        name = "Box";
    }

    void collision_process()
    {
        CollObj::collision_process();

        if (position.x < 0)
        {
            start_delete();
        }
    }

    void collide_begin(CollObj* other)
    {
        Mouse* mse_other = dynamic_cast<Mouse*>(other);

        if (mse_other != nullptr)
        {
            start_delete();
        }
    }
};

int main()
{
    b2WorldDef world_def = b2DefaultWorldDef();
    world_def.gravity = pos(-100, 0);

    b2WorldId coll_world = b2CreateWorld(&world_def);
    set_current_coll_world(coll_world);

    b2SetLengthUnitsPerMeter(B2_SCALE);
    b2World_SetMaximumLinearSpeed(coll_world, B2_MAX_MOVEMENT);

    game* gameplay = new game;
    set_current_game(gameplay);

    gameplay->coll_world = coll_world;

    Mouse* mse = new Mouse;
    DrawObj* spri = new DrawObj;

    SDL_Surface* surf = SDL_LoadBMP("sample2.bmp");
    SDL_Texture* bmp = SDL_CreateTextureFromSurface(gameplay->game_window->renderer, surf);
    SDL_DestroySurface(surf);

    spri->set_sprite(bmp, false);

    mse->set_position(pos(250,250));

    mse->add_child(spri);

    gameplay->root->add_child(mse);

    StaticObj* huh1 = new StaticObj;
    gameplay->root->add_child(huh1);

    pos center(250,250);

    const int total = 1000;
    for (int x = 0; x < total; ++x)
    {
        DynamObj* obj = new DynamObj;
        DrawObj* sprite = new DrawObj;
    
        sprite->set_sprite(bmp, false);

        obj->set_position(center + pos(x * size * 3, 0));
    
        obj->add_child(sprite);
    
        gameplay->root->add_child(obj);
    }

    gameplay->start();

    return 0;
}