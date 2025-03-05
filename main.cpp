#include <iostream>
#include <vector>

#include "game.h"
#include "drawobj.h"
#include "collobj.h"

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

class DynamObj;

class StaticObj : public CollObj
{
public:
    StaticObj()
    {
        collision_body = get_current_coll_world()->CreateBody(&collision_def);

        b2Vec2 points[] = {{0.0, 0.0},{0.1, 0.0}, {0.0,500.0}};

        b2PolygonShape roundedTriangle;
        roundedTriangle.Set(points,3);
        
        collision_body->CreateFixture(&roundedTriangle,0.0);

        b2Vec2 points2[] = {{0,-500}, {10000,-500}, {10000,0}, {0,0}};

        b2PolygonShape sqr;
        sqr.Set(points2,4);
        
        collision_body->CreateFixture(&sqr,0.0);

        b2Vec2 points3[] = {{0,500}, {10000,500}, {10000,600}, {0,600}};

        b2PolygonShape sqr2;
        sqr2.Set(points3,4);
        
        collision_body->CreateFixture(&sqr2,0.0);
    }
};

#define size 6

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
        box.m_radius = size;
        
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &box;
        fixtureDef.density = 1;
        fixtureDef.friction = 0;
        fixtureDef.restitution = 1;

        collision_body->CreateFixture(&fixtureDef);
    }
};

class Mouse : public CollObj
{
public:
    Mouse()
    {
        collision_def.type = b2_kinematicBody;
        collision_def.bullet = true;

        collision_body = get_current_coll_world()->CreateBody(&collision_def);

        b2CircleShape box;
        box.m_p.Set(0,0);
        box.m_radius = size;
        
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &box;
        fixtureDef.density = 50;
        fixtureDef.friction = 1;
        fixtureDef.restitution = 2;

        collision_body->CreateFixture(&fixtureDef);
    }

    void process(double delta)
    {
        int x,y;
        al_get_mouse_cursor_position(&x,&y);

        int src_x,src_y;
        al_get_window_position(get_current_game()->game_window->display,&src_x,&src_y);

        position = pos(x - src_x, y - src_y);

        CollObj::process(delta);
    }
};

int main()
{
    b2World* coll_world = new b2World(b2Vec2(-100.0,0));
    set_current_coll_world(coll_world);

    game* gameplay = new game;
    set_current_game(gameplay);

    gameplay->coll_world = coll_world;

    gameplay->root->add_child(new StaticObj);
    gameplay->root->add_child(new Mouse);

    pos position = {200,250};
    
    for (int x = 0; x < 1000; x++)
    {
        DynamObj* obj = new DynamObj;
        DrawObj* spr = new DrawObj;

        ALLEGRO_BITMAP* bmp = al_create_bitmap(2 * size,2 * size);

        al_set_target_bitmap(bmp);
        al_draw_circle(size,size,size,WHITE,1);

        spr->set_sprite(bmp);

        obj->add_child(spr);
        gameplay->root->add_child(obj);

        obj->set_position(position + pos(size * 2 * x,0));
    }

    gameplay->start();

    return 0;
}