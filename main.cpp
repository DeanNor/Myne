#include <iostream>
#include <vector>

#include "game.hpp"
#include "drawobj.hpp"
#include "collobj.hpp"

pos to_tileset(pos value, pos tile_size);

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


// USER DEFINED STUFF ISH
#define HUH 9000

class StaticObj : public CollObj
{
public:
    StaticObj()
    {
        collision_def.type = b2_staticBody;
        collision_body = get_current_coll_world()->CreateBody(&collision_def);

        b2PolygonShape box;
        box.SetAsBox(HUH / B2_SCALE,HUH / B2_SCALE);
        
        collision_body->CreateFixture(&box,0.0);
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
    }
};

class Mouse : public CollObj
{
public:
    Mouse()
    {
        collision_def.type = b2_kinematicBody;

        collision_body = get_current_coll_world()->CreateBody(&collision_def);

        b2PolygonShape box;
        box.SetAsBox(30.0f / B2_SCALE, 30.0f / B2_SCALE);
        
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &box;
        fixtureDef.density = 10;
        fixtureDef.friction = 1;
        fixtureDef.restitution = 0.25;

        collision_body->CreateFixture(&fixtureDef);

        collision_body->SetAngularVelocity(500.0f / B2_SCALE);
    }

    void process(double delta)
    {
        int x,y;
        al_get_mouse_cursor_position(&x,&y);

        int src_x,src_y;
        al_get_window_position(get_current_game()->game_window->display,&src_x,&src_y);

        pos glo_pos = global_position; 

        collision_body->SetLinearVelocity(b2Vec2((x - src_x - glo_pos.x) * 60 / B2_SCALE, (y - src_y - glo_pos.y) * 60 / B2_SCALE));

        CollObj::process(delta);
    }
};

int main()
{
    b2World* coll_world = new b2World(b2Vec2(-100.0 / B2_SCALE,0));
    set_current_coll_world(coll_world);

    game* gameplay = new game;
    set_current_game(gameplay);

    gameplay->coll_world = coll_world;

    Mouse* mse = new Mouse;
    DrawObj* spri = new DrawObj;

    ALLEGRO_BITMAP* bimp = al_create_bitmap(2 * 30,2 * 30);

    al_set_target_bitmap(bimp);
    al_draw_rectangle(1,1,59,59,WHITE,1);

    spri->set_sprite(bimp);

    mse->add_child(spri);

    gameplay->root->add_child(mse);

    StaticObj* huh1 = new StaticObj;
    huh1->set_position(pos(-HUH,HUH));
    gameplay->root->add_child(huh1);

    StaticObj* huh2 = new StaticObj;
    huh2->set_position(pos(HUH,-HUH));
    gameplay->root->add_child(huh2);

    StaticObj* huh3 = new StaticObj;
    huh3->set_position(pos(HUH,500 + HUH));
    gameplay->root->add_child(huh3);

    pos position = {200,250};
    double amount = 2000;
    double start = 0.25;
    for (double x = 0; x < amount; x++)
    {
        DynamObj* obj = new DynamObj;
        DrawObj* spr = new DrawObj;

        ALLEGRO_BITMAP* bmp = al_create_bitmap(2 * size, 2 * size);

        al_set_target_bitmap(bmp);
        al_draw_circle(size,size,size,al_premul_rgba_f(1, 1, 1, x / amount),1);
        al_draw_line(size,size,size + size, size, al_map_rgb_f(1,1,1),1);

        spr->set_sprite(bmp);

        obj->add_child(spr);
        gameplay->root->add_child(obj);
        

        obj->set_position(position + pos(size * 2 * x + rand() % 50 - 25,rand() % 50 - 25));
    }

    gameplay->start();

    return 0;
}