
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
private:
    display* game_window = nullptr;

    Process* root = nullptr;

    // 255 depth values for both draws and overlay
    std::vector<DrawObj*> draws[std::numeric_limits<unsigned char>::max() + 1];

    std::vector<CollObj*> collisions = {};
    std::vector<Process*> deletes = {};

    b2WorldId coll_world;
    bool physics = false;

    bool running = true;

    double fps = 60;
    double spf = 1.0 / fps; // Seconds per frame
    Uint64 fpsticks = 1000 / fps; // MSeconds between ticks

    double coll_fps = 60;
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

    int coll_iterations = 10;

    mouse_state mouse;
    keyboard_state keyboard;

public:
    game(const char* name, SDL_WindowFlags flags, pos window_size);

    virtual ~game();

    virtual bool frame();

    void wait_for_next_task();

    void view_events();

    void process_event(SDL_Event event);

    void update_run_data();

    void run_processes();

    void run_collision();

    void run_frame();

    void start();

    void process();

    void draw() const;

    void end_delete();

    void add_to_deletes(Process* who);

    void add_to_collisions(CollObj* who);

    void add_to_draws(DrawObj* who, const unsigned char& depth);

    void remove_from_draws(DrawObj* who, const unsigned char& depth);

    void remove_from_collisions(CollObj* who);

    display* get_game_window() const 
    {
        return game_window;
    }

    void set_root(Process* new_root)
    {
        root = new_root;
    }

    Process* get_root() const
    {
        return root;
    }

    void set_coll_world(b2WorldId new_coll_world)
    {
        coll_world = new_coll_world;
    }

    b2WorldId get_coll_world() const
    {
        return coll_world;
    }

    void set_physics(bool has)
    {
        physics = has;
    }

    bool has_physics() const
    {
        return physics;
    }

    void set_running(bool is_running)
    {
        running = is_running;
    }

    void set_fps(double val)
    {
        fps = val;
        spf = 1.0 / fps;
        fpsticks = 1000 / fps;
    }

    double get_fps() const
    {
        return fps;
    }

    double get_spf() const
    {
        return spf;
    }
    
    Uint64 get_fpsticks() const
    {
        return fpsticks;
    }

    void set_coll_fps(double val)
    {
        coll_fps = val;
        coll_spf = 1.0 / coll_fps;
        collticks = 1000 / coll_fps;
    }

    double get_coll_fps() const
    {
        return coll_fps;
    }

    double get_coll_spf() const
    {
        return coll_spf;
    }
    
    Uint64 get_coll_fpsticks() const
    {
        return collticks;
    }

    void set_frame_fps(double val)
    {
        frame_fps = val;
        frame_spf = 1.0 / frame_fps;
        frameticks = 1000 / frame_fps;
    }

    double get_frame_fps() const
    {
        return frame_fps;
    }

    double get_frame_spf() const
    {
        return frame_spf;
    }
    
    Uint64 get_frame_fpsticks() const
    {
        return frameticks;
    }

    // FPS as known by the coll engine
    void set_coll_progression(float new_coll_progression)
    {
        coll_progression = new_coll_progression;
    }

    float get_coll_progression()
    {
        return coll_progression;
    }

    double get_delta() const
    {
        return delta;
    }

    void set_coll_iterations(int iterations)
    {
        coll_iterations = iterations;
    }

    int get_coll_iterations() const
    {
        return coll_iterations;
    }

    mouse_state& get_mouse()
    {
        return mouse;
    }

    keyboard_state& get_keyboard()
    {
        return keyboard;
    }

private:
    bool __remove_from_draws(DrawObj* who, const unsigned char& depth);


    bool __remove_from_overlay_draws(BlendObj* who, const unsigned char& depth);
};

void set_current_coll_world(b2WorldId world);
b2WorldId get_current_coll_world();

void set_current_game(game* new_world);
game* get_current_game();
