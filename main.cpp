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

public:
    void draw()
    {
        if (redraw)
        {
        ALLEGRO_BITMAP* main_bitmap = al_get_target_bitmap();
        
            al_set_target_bitmap(tilemap);

            for (pos tile : tiles)
            {
                al_draw_bitmap(sprite,tile.x,tile.y,0);
            }

        al_set_target_bitmap(main_bitmap);
       
        }
 al_draw_bitmap(tilemap,position.x,position.y,0);
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
        collision_def.position.Set(0,0);
        collision_body = get_current_coll_world()->CreateBody(&collision_def);

        b2Vec2 points[] = {{0.0, 0.0}, {200.0, 0.0}, {0.0,500.0}};

        b2PolygonShape roundedTriangle;
        roundedTriangle.Set(points,3);
        
        collision_body->CreateFixture(&roundedTriangle,0.0);

        b2Vec2 points2[] = {{0,-500}, {500,-500}, {500,0}, {0,0}};

        b2PolygonShape sqr;
        sqr.Set(points2,4);
        
        collision_body->CreateFixture(&sqr,0.0);

        b2Vec2 points3[] = {{0,500}, {500,500}, {500,600}, {0,600}};

        b2PolygonShape sqr2;
        sqr2.Set(points3,4);
        
        collision_body->CreateFixture(&sqr2,0.0);
    }
};

class DynamObj : public CollObj
{
public:
    DynamObj()
    {
        collision_def.position.Set(150,0);
        collision_def.type = b2_dynamicBody;
        collision_body = get_current_coll_world()->CreateBody(&collision_def);

        b2CircleShape box;
        box.m_p.Set(0,0);
        box.m_radius = 2.5;
        
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &box;
        fixtureDef.density = 10;
        fixtureDef.friction = 0.7;
        fixtureDef.restitution = 0.5;

        collision_body->CreateFixture(&fixtureDef);
    }
};

int main()
{
    b2World* coll_world = new b2World(b2Vec2(-50.0,0));
    set_current_coll_world(coll_world);

    game* gameplay = new game;
    set_current_game(gameplay);

    gameplay->coll_world = coll_world;

    gameplay->root->add_child(new StaticObj);

    pos a = {600,50};

    for (int x = 0; x < 5; ++x)
    {
        DynamObj* dynam = new DynamObj;
        gameplay->root->add_child(dynam);

        dynam->set_position(a + pos(x * 5,0));

        DrawObj* spr = new DrawObj();

        ALLEGRO_BITMAP* bitmap = al_create_bitmap(5,5);
        al_set_target_bitmap(bitmap);
        al_clear_to_color(WHITE);

        spr->set_sprite(bitmap);
        dynam->add_child(spr);
    }

    std::cout << "Start" << std::endl;

    gameplay->start();

    return 0;
}