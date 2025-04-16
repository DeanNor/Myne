
#pragma once

#include "display.hpp"

#include <box2d/box2d.h>

#include <vector>

class Process;
class DrawObj;
class BlendObj;
class CollObj;

struct mouse_state
{
public:
    pos position;

    bool down = false;

    mouse_state() = default;

    mouse_state(const mouse_state&) = delete;
};

struct game
{
    display* game_window = nullptr;

    Process* root = nullptr;

    std::vector<DrawObj*> draws = {};
    std::vector<BlendObj*> overlay_draws = {};
    std::vector<CollObj*> collisions = {};
    std::vector<Process*> deletes = {};

    b2WorldId coll_world;
    bool physics = true;

    bool running = true;

    double fps = 60;
    double spf = 1.0 / fps; // Seconds between ticks
    Uint64 fpsticks = 1000 / fps; // MSeconds between ticks

    Uint64 total_ticks = 0; // Internal clock

    int coll_iterations = 4;

    mouse_state mouse;

    game();

    ~game();

    bool frame();

    void start();

    void process();

    void collision_process();

    void draw();

    void draw_overlay();

    void end_delete();

    void remove_from_draws(DrawObj* who);

    void remove_from_overlay_draws(BlendObj* who);

    void remove_from_collisions(CollObj* who);
};

void set_current_coll_world(b2WorldId world);
b2WorldId get_current_coll_world();

void set_current_game(game* new_world);
game* get_current_game();