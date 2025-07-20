
#include "game.hpp"

#include "process.hpp"
#include "drawobj.hpp"
#include "blendobj.hpp"
#include "collobj.hpp"

#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_init.h>

#include <algorithm>
#include <limits>

game::game(const char* name, SDL_WindowFlags flags)
{
    if (!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO))
    {
        std::cout << "HUH INIT FAIL?\n";
        // TODO If this happens, blow up your computer

        std::cout << SDL_GetError() << '\n' << '\n';
    }

    game_window = new display({1000, 500}, name, flags);

    root = new Process; // TODO remove

    /*std::cout.setf(std::ios::fixed | std::ios::showpoint);
    std::cout.precision(80);*/
}

game::~game()
{
    if (game_window != nullptr)
    {
        delete game_window;
        game_window = nullptr;
    }

    SDL_Quit();
}

bool game::frame()
{
    view_events();
    run_processes();

    wait_for_frame();
    finish_processes();

    return running;
}

void game::view_events()
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        process_event(event);
    }
}

void game::process_event(SDL_Event event)
{
    switch (event.type)
    {
    case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
        if (event.window.windowID == SDL_GetWindowID(game_window->window))
        {
            running = false;
        }
        
    case SDL_EVENT_QUIT:
        running = false;

    case SDL_EVENT_WINDOW_RESIZED:
        game_window->update_size();
        break;

    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP:
        mouse.recheck(event.button);
        break;
    }
}

void game::run_processes()
{
    // Update mouse position
    float x,y;
    SDL_GetMouseState(&x,&y);
    mouse.position = pos(x,y) + game_window->center - game_window->half_size;

    delay = (long double)(SDL_GetTicksNS() - total_delay) / NSPS;
    total_delay = SDL_GetTicksNS();

    process();
    
    if (physics)
    {
        collision_process();
    }

    game_window->prepare_screen();
    draw();

    draw_overlay();
}

void game::wait_for_frame()
{
    const Uint64 ticks = SDL_GetTicks() - total_ticks;
    if (ticks < fpsticks)
    {
        //Wait remaining time
        SDL_Delay(fpsticks - ticks);
    }

    total_ticks = SDL_GetTicks();
}

void game::finish_processes()
{
    game_window->push_screen();

    end_delete();
}

void game::start()
{
    while(frame());

    delete game_window;
    game_window = nullptr;
}

void game::process()
{
    if (root != nullptr)
    {
        root->_process(delay);
    }

    else
    {
        std::cout << "No Root" << std::endl;
        running = false;
    }
}

void game::collision_process() const
{
    // TODO timeStep should be constant, 0.016 prolly
    b2World_Step(coll_world, coll_spf, coll_iterations);

    const b2SensorEvents sensors = b2World_GetSensorEvents(coll_world);

    for (int x = 0; x < sensors.beginCount; ++x)
    {
        b2SensorBeginTouchEvent* event = sensors.beginEvents + x;
        CollObj::SensorBegin(event->sensorShapeId, event->visitorShapeId);
    }

    for (int y = 0; y < sensors.endCount; ++y)
    {
        b2SensorEndTouchEvent* event = sensors.endEvents + y;
        CollObj::SensorEnd(event->sensorShapeId, event->visitorShapeId);
    }

    const b2ContactEvents contacts = b2World_GetContactEvents(coll_world);

    for (int x = 0; x < contacts.beginCount; ++x)
    {
        b2ContactBeginTouchEvent* event = contacts.beginEvents + x;
        CollObj::CollisionBegin(event->shapeIdA, event->shapeIdB);
    }

    for (int y = 0; y < contacts.endCount; ++y)
    {
        b2ContactEndTouchEvent* event = contacts.endEvents + y;
        CollObj::CollisionEnd(event->shapeIdA, event->shapeIdB);
    }

    for (CollObj* collision : collisions)
    {
        collision->collision_process();
    }
}

void game::draw() const
{
    const pos origin = game_window->center - game_window->half_size;
    for (unsigned char x = 0; x < std::numeric_limits<unsigned char>::max(); x++)
    {
        const std::vector<DrawObj*>& layer = draws[x];
        for (DrawObj* object : layer)
        {
            object->draw(origin);
        }
    }
}

void game::draw_overlay() const
{
    const pos origin = game_window->center - game_window->half_size;
    for (unsigned char x = 0; x < std::numeric_limits<unsigned char>::max(); x++)
    {
        const std::vector<BlendObj*>& layer = overlay_draws[x];
        for (BlendObj* object : layer)
        {
            object->draw_overlay(origin);
        }
    }
}

void game::end_delete()
{
    if (deletes.size() > 0)
    {
        for (Process* to_delete : deletes)
        {
            delete to_delete;
        }

        deletes.clear();
    }
}

void game::add_to_draws(DrawObj* who, const unsigned char& depth)
{
    draws[depth].push_back(who);
}

bool game::__remove_from_draws(DrawObj* who, const unsigned char& depth)
{
    std::vector<DrawObj*>& layer = draws[depth];
    auto index = std::find(layer.begin(), layer.end(), who);

    if (index == layer.end()) return false;

    layer.erase(index);
    return true;
}

void game::remove_from_draws(DrawObj* who, const unsigned char& depth)
{
    if (__remove_from_draws(who,depth)) return;

    for (unsigned char x = depth + 1; x != depth; x++) // Uses unsigned looping to get all
    {
        if (__remove_from_draws(who, x)) return;
    }

    std::cout << "Huh Draws" << std::endl;
}

void game::add_to_overlay_draws(BlendObj* who, const unsigned char& depth)
{
    overlay_draws[depth].push_back(who);
}

bool game::__remove_from_overlay_draws(BlendObj* who, const unsigned char& depth)
{
    std::vector<BlendObj*>& layer = overlay_draws[depth];
    auto index = std::find(layer.begin(), layer.end(), who);

    if (index == layer.end()) return false;

    layer.erase(index);
    return true;
}

void game::remove_from_overlay_draws(BlendObj* who, const unsigned char& depth)
{
    if (__remove_from_overlay_draws(who,depth)) return;

    for (unsigned char x = depth + 1; x != depth; x++) // Uses unsigned looping to get all
    {
        if (__remove_from_overlay_draws(who, x)) return;
    }

    std::cout << "HUH Overlay Draws" << std::endl;
}

void game::remove_from_collisions(CollObj* who)
{
    const std::vector<CollObj*>::iterator index = std::find(collisions.begin(), collisions.end(), who);

    if (index != collisions.end())
    {
        collisions.erase(index);
    }

    else
    {
        std::cout << "HUH Collision" << std::endl; // Error, object seems already deleted and has no draw calls
    }
}

// -------------------------------------- Storage of current stuffs
b2WorldId coll_world;
game* gameplay = nullptr;

void set_current_coll_world(b2WorldId world)
{
    coll_world = world;
}

b2WorldId get_current_coll_world()
{
    return coll_world;
}

void set_current_game(game* new_game)
{
    gameplay = new_game;
}

game* get_current_game()
{
    return gameplay;
}