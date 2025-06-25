
#pragma once

#include "display.hpp"

#include <box2d/box2d.h>

#include <vector>

class Process;
class DrawObj;
class BlendObj;
class CollObj;

const static long double MSPS = 1000.;
const static long double NSPS = 1.e+9;

struct mouse_state // TODO Proper input handling and all mouse buttons
{
public:
    pos position;

    bool ldown = false;
    bool mdown = false;
    bool rdown = false;

    mouse_state() = default;

    mouse_state(const mouse_state&) = delete;

    void recheck(SDL_MouseButtonEvent& mse)
    {
        switch(mse.button)
        {
        case SDL_BUTTON_LEFT:
            ldown = mse.down;
            break;

        case SDL_BUTTON_MIDDLE:
            mdown = mse.down;
            break;
            
        case SDL_BUTTON_RIGHT:
            rdown = mse.down;
            break;
        }
    }
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

    double coll_spf = 1 / 60.0;

    Uint64 total_ticks = 0; // Internal clock
    Uint64 total_delay = 0; // Internal clock #2, uses NS //TODO remove the #2
    double delay; // Delta time

    int coll_iterations = 4;

    mouse_state mouse;

    game(const char* name, SDL_WindowFlags flags);

    virtual ~game();

    virtual bool frame();

    void view_events();

    void process_event(SDL_Event event);

    void run_processes();

    void wait_for_frame();

    void finish_processes();

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