
#pragma once

#ifndef GAME_H
#define GAME_H

#include "window.h"

#include <box2d/box2d.h>

#include <vector>

class Process;
class DrawObj;
class CollObj;

struct game
{
    window* game_window = nullptr;
    
    ALLEGRO_TIMER* timer = nullptr;
    ALLEGRO_EVENT_QUEUE* queue = nullptr;

    Process* root = nullptr;
    std::vector<DrawObj*> draws = {};
    std::vector<CollObj*> collisions = {};

    b2World* coll_world = nullptr;

    bool running = true;

    int fps = 60;
    double spf = 1.0 / (double)fps;

    int velocity_iterations = 6;
    int position_iterations = 2;

    game();

    ~game();

    bool frame();

    void start();

    void process();

    void collision_process();

    void draw();
};

void set_current_coll_world(b2World* world);
b2World* get_current_coll_world();

void set_current_game(game* new_world);
game* get_current_game();

#endif