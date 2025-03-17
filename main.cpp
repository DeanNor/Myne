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
#define HUH 9000

class StaticObj : public CollObj
{
public:
    StaticObj()
    {
        collision_def.type = b2_staticBody;
        collision_body = get_current_coll_world()->CreateBody(&collision_def);

        b2PolygonShape box1;
        box1.SetAsBox(HUH / B2_SCALE,HUH / B2_SCALE, pos(-HUH / B2_SCALE, HUH / B2_SCALE), 0.0f);

        b2PolygonShape box2;
        box2.SetAsBox(HUH / B2_SCALE,HUH / B2_SCALE, pos(HUH / B2_SCALE, -HUH / B2_SCALE), 0.0f);

        b2PolygonShape box3;
        box3.SetAsBox(HUH / B2_SCALE,HUH / B2_SCALE, pos(HUH / B2_SCALE, (500 + HUH) / B2_SCALE), 0.0f);
        
        collision_body->CreateFixture(&box1,0.0);
        collision_body->CreateFixture(&box2,0.0);
        collision_body->CreateFixture(&box3,0.0);
    }
};

#define size 6
#define b2size size / B2_SCALE

class DynamObj : public CollObj
{
public:
    DynamObj()
    {
        collision_def.type = b2_dynamicBody;
        collision_def.allowSleep = true;

        collision_body = get_current_coll_world()->CreateBody(&collision_def);

        b2CircleShape box;
        box.m_p.Set(0,0);
        box.m_radius = b2size;
        
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &box;
        fixtureDef.density = 1;
        fixtureDef.friction = 1;
        fixtureDef.restitution = 0.25;

        collision_body->CreateFixture(&fixtureDef);
    }

    void collision_process(double delta)
    {
        CollObj::collision_process(delta);

        if (position.x < 0)
        {
            position = pos(250,250);
        }

        //std::cout << position << std::endl;
    }
};

float len = 30, hgt = 400;
class Mouse : public CollObj
{
public:
    Mouse()
    {
        collision_def.type = b2_kinematicBody;

        collision_body = get_current_coll_world()->CreateBody(&collision_def);

        b2PolygonShape box;
        box.SetAsBox(len / 2.0f / B2_SCALE, hgt / 2.0f / B2_SCALE);
        
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &box;
        fixtureDef.density = 10;
        fixtureDef.friction = 1;
        fixtureDef.restitution = 0.25;

        collision_body->CreateFixture(&fixtureDef);

        collision_body->SetAngularVelocity(1000.0f / B2_SCALE);
    }

    void process(double delta)
    {
        float x,y;
        SDL_GetGlobalMouseState(&x,&y);

        pos glo_pos = global_position;
        
        int src_x, src_y;
        SDL_GetWindowPosition(get_current_game()->game_window->window, &src_x, &src_y);

        collision_body->SetLinearVelocity(b2Vec2((x - glo_pos.x - src_x) * 60 / B2_SCALE, (y - glo_pos.y - src_y) * 60 / B2_SCALE));

        CollObj::process(delta);
    }
};


void DrawCircle(SDL_Renderer* renderer, int32_t centerX, int32_t centerY, int32_t radius) {
    int32_t x = radius;
    int32_t y = 0;
    int32_t diameter = (radius * 2);
    int32_t dx = 1;
    int32_t dy = 1;
    int32_t delta = 1 - radius;

    while (x >= y) {
        SDL_RenderPoint(renderer, centerX + x, centerY - y);
        SDL_RenderPoint(renderer, centerX + x, centerY + y);
        SDL_RenderPoint(renderer, centerX - x, centerY - y);
        SDL_RenderPoint(renderer, centerX - x, centerY + y);
        SDL_RenderPoint(renderer, centerX + y, centerY - x);
        SDL_RenderPoint(renderer, centerX + y, centerY + x);
        SDL_RenderPoint(renderer, centerX - y, centerY - x);
        SDL_RenderPoint(renderer, centerX - y, centerY + x);

        if (delta < 0) {
            delta += 2 * y + 1;
            y++;
        }
        if (delta > 0) {
            delta -= 2 * x + 1;
            x--;
        }
        if (delta == 0) {
            delta += 2 * (y - x + 1);
            x--;
            y++;
        }
        dx = 1;
        dy = 1;
    }
}

int main()
{
    b2World* coll_world = new b2World(b2Vec2(-100.0 / B2_SCALE,0));
    set_current_coll_world(coll_world);

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

    spri->set_sprite(bimp);

    mse->add_child(spri);

    gameplay->root->add_child(mse);

    StaticObj* huh1 = new StaticObj;
    gameplay->root->add_child(huh1);

    pos position = {40,250};
    double amount = 1000;
    double start = 0.25;
    for (double x = 0; x < amount; x++)
    {
        DynamObj* obj = new DynamObj;
        DrawObj* spr = new DrawObj;

        SDL_Texture* bmp = SDL_CreateTexture(gameplay->game_window->renderer, SDL_PIXELFORMAT_ABGR64_FLOAT, SDL_TEXTUREACCESS_TARGET, size * 2 + 1, size * 2 + 1);
        std::cout << bmp << std::endl;
        SDL_SetRenderDrawColor(gameplay->game_window->renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
        SDL_SetRenderTarget(gameplay->game_window->renderer, bmp);

        DrawCircle(gameplay->game_window->renderer, size, size, size);

        SDL_SetRenderTarget(gameplay->game_window->renderer, nullptr);
        SDL_SetRenderDrawColor(gameplay->game_window->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

        spr->set_sprite(bmp);

        obj->add_child(spr);
        gameplay->root->add_child(obj);

        obj->set_position(position + pos(size * 2 * x + rand() % 50 - 25,rand() % 50 - 25));
    }

    gameplay->start();

    return 0;
}