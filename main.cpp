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
#define HUH 250

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
        fixtureDef.restitution = 0.25;

        b2CreatePolygonShape(collision_body, &fixtureDef, &box);
    }

    void collision_process()
    {
        CollObj::collision_process();

        if (position.x < 0)
        {
            start_delete();
        }
    }
};

float len = 30, hgt = 400;
class Mouse : public CollObj
{
public:
    Mouse()
    {
        collision_def.type = b2_kinematicBody;

        collision_body = b2CreateBody(get_current_coll_world(), &collision_def);

        b2Polygon box = b2MakeBox(len / 2.0f, hgt / 2.0f);
        
        b2ShapeDef fixtureDef = b2DefaultShapeDef();
        fixtureDef.density = 10;
        fixtureDef.friction = 1;
        fixtureDef.restitution = 0;

        b2CreatePolygonShape(collision_body, &fixtureDef, &box);
    }

    void process(double delta)
    {
        float x,y;
        SDL_GetGlobalMouseState(&x,&y);

        pos glo_pos = global_position;
        
        int src_x, src_y;
        SDL_GetWindowPosition(get_current_game()->game_window->window, &src_x, &src_y);

        b2Vec2 target = {(float)(x - glo_pos.x - src_x) * 60.0f, (float)(y - glo_pos.y - src_y) * 60.0f};
        b2Body_SetLinearVelocity(collision_body, target);

        b2Body_SetAngularVelocity(collision_body, 5.0f);

        CollObj::process(delta);
    }
};

int main()
{
    b2WorldDef coll_def = b2DefaultWorldDef();
    coll_def.gravity = b2Vec2{-100.0f,0.0f};
    b2WorldId coll_world = b2CreateWorld(&coll_def);
    set_current_coll_world(coll_world);

    b2SetLengthUnitsPerMeter(B2_SCALE);

    game* gameplay = new game;
    set_current_game(gameplay);

    gameplay->coll_world = coll_world;

    Mouse* mse = new Mouse;
    DrawObj* spri = new DrawObj;

    SDL_Texture* bimp = SDL_CreateTexture(gameplay->game_window->renderer, SDL_PIXELFORMAT_ABGR64_FLOAT, SDL_TEXTUREACCESS_TARGET, len,hgt);
    SDL_SetRenderDrawColor(gameplay->game_window->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_SetRenderTarget(gameplay->game_window->renderer, bimp);
    
    SDL_FRect rext = pos::make_SDL_FRect(pos(len / 2.0,hgt / 2.0), pos(len / 2.0,hgt / 2.0));
    SDL_RenderRect(gameplay->game_window->renderer, &rext);

    SDL_SetRenderTarget(gameplay->game_window->renderer, nullptr);
    SDL_SetRenderDrawColor(gameplay->game_window->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

    spri->set_sprite(bimp, true);

    mse->set_position(pos(250,250));

    mse->add_child(spri);

    gameplay->root->add_child(mse);

    StaticObj* huh1 = new StaticObj;
    gameplay->root->add_child(huh1);

    pos center(250,250);
    const int count = 2000;
    for (int iterx = 0; iterx < count; ++iterx)
    {
        DynamObj* obj = new DynamObj;
        DrawObj* sprite = new DrawObj;
        
        SDL_Surface* surf = SDL_LoadBMP("sample2.bmp");
        SDL_Texture* bmp = SDL_CreateTextureFromSurface(gameplay->game_window->renderer, surf);
        SDL_DestroySurface(surf);
    
        sprite->set_sprite(bmp, true);

        obj->set_position(center + pos(iterx * size * 3, 0));
    
        obj->add_child(sprite);
    
        gameplay->root->add_child(obj);
    }

    gameplay->start();

    return 0;
}