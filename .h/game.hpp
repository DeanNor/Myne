
#pragma once

#include "display.hpp"

#include <box2d/box2d.h>

#include <vector>
#include <thread>

class Process;
class DrawObj;
class CollObj;

struct game
{
    display* game_window = nullptr;

    Process* root = nullptr;

    std::vector<DrawObj*> draws = {};
    std::vector<CollObj*> collisions = {};
    std::vector<Process*> deletes = {};

    b2World* coll_world = nullptr;

    bool running = true;

    int fps = 60;
    double spf = 1.0 / double(fps);
    Uint64 fpsticks = 1000 / fps;

    Uint64 total_ticks = 0;

    int velocity_iterations = 2;
    int position_iterations = 2;

    game();

    ~game();

    bool frame();

    void start();

    void process();

    void collision_process();

    void draw();

    void end_delete();
};

void set_current_coll_world(b2World* world);
b2World* get_current_coll_world();

void set_current_game(game* new_world);
game* get_current_game();