
#pragma once

#include "SDL3/SDL_events.h"
#include "box2d/id.h"

#include ".hpp/display.hpp"

#include "input.hpp"

#include <limits>
#include <vector>

class Process;
class DrawObj;
class BlendObj;
class CollObj;

const static long double MSPS = 1000.;
const static long double NSPS = 1.e9;

struct game
{
    display* game_window = nullptr;

    Process* root = nullptr;

    // 255 depth values for both draws and overlay
    std::vector<DrawObj*> draws[std::numeric_limits<unsigned char>::max() + 1];
    std::vector<BlendObj*> overlay_draws[std::numeric_limits<unsigned char>::max() + 1];

    std::vector<CollObj*> collisions = {};
    std::vector<Process*> deletes = {};

    b2WorldId coll_world;
    bool physics = true;

    bool running = true;

    double fps = 60;
    double spf = 1.0 / fps; // Seconds per frame
    Uint64 fpsticks = 1000 / fps; // MSeconds between ticks

    double coll_fps = 120;
    double coll_spf = 1 / coll_fps;
    Uint64 collticks = 1000 / coll_fps;

    double frame_fps = 60;
    double frame_spf = 1 / frame_fps;
    Uint64 frameticks = 1000 / frame_fps;

    Uint64 total_ticks = 0; // Internal clocks
    Uint64 total_coll_ticks = 0;
    Uint64 total_frame_ticks = 0;

    float coll_progression = spf;

    Uint64 total_delay = 0; // Internal clock #2, uses NS //TODO remove the #2
    double delta; // Delta time for last frame, before all other calls.

    int coll_iterations = 4;

    mouse_state mouse;
    keyboard_state keyboard;

    game(const char* name, SDL_WindowFlags flags);

    virtual ~game();

    virtual bool frame();

    void wait_for_next_task();

    void view_events();

    void process_event(SDL_Event event);

    void run_processes();

    void run_collision();

    void run_frame();

    void start();

    void process();

    void draw() const;

    void draw_overlay() const;

    void end_delete();

    void add_to_draws(DrawObj* who, const unsigned char& depth);

    void remove_from_draws(DrawObj* who, const unsigned char& depth);

    void add_to_overlay_draws(BlendObj* who, const unsigned char& depth);

    void remove_from_overlay_draws(BlendObj* who, const unsigned char& depth);

    void remove_from_collisions(CollObj* who);

private:
    bool __remove_from_draws(DrawObj* who, const unsigned char& depth);


    bool __remove_from_overlay_draws(BlendObj* who, const unsigned char& depth);
};

void set_current_coll_world(b2WorldId world);
b2WorldId get_current_coll_world();

void set_current_game(game* new_world);
game* get_current_game();
