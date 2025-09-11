
#include ".hpp/game.hpp"

#include ".hpp/process.hpp"
#include ".hpp/drawobj.hpp"
#include ".hpp/blendobj.hpp"
#include ".hpp/collobj.hpp"
#include "SDL3/SDL_events.h"

#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_init.h>
#include <limits>

#include ".hpp/ranges.hpp"

game::game(const char* name, SDL_WindowFlags flags, pos window_size)
{
    if (!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO))
    {
        std::cout << "HUH INIT FAIL?\n";
        // TODO If this happens, blow up your computer

        std::cout << SDL_GetError() << '\n' << '\n';
    }

    game_window = new display(window_size, name, flags);

    root = new Process; // TODO decide to remove

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
    Uint64 tick_count = SDL_GetTicks();
    Uint64 ticks = tick_count - total_ticks;
    Uint64 coll_ticks = tick_count - total_coll_ticks;
    Uint64 frame_ticks = tick_count - total_frame_ticks;

    int count = floor(ticks / fpsticks);
    int coll_count = floor(coll_ticks / collticks);
    bool frame_count = frame_ticks >= frameticks;

    if (count > 0 || coll_count > 0 || frame_count)
    {
        do
        {
            if (count > 0)
            {
                total_ticks = tick_count;

                run_processes();
                --count;
            }

            if (coll_count > 0)
            {
                total_coll_ticks = tick_count;

                run_collision();
                --coll_count;
            }

        } while (count > 0 || coll_count > 0);

        if (frame_count)
        {
            run_frame();

            total_frame_ticks = tick_count;
        }

        end_delete();
    }

    else
    {
        SDL_Delay(get_lowest(3, fpsticks - ticks, collticks - coll_ticks, frameticks - frame_ticks));
    }

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
        if (event.window.windowID == SDL_GetWindowID(game_window->get_window()))
        {
            running = false;
        }
        break;
        
    case SDL_EVENT_QUIT:
        running = false;
        break;

    case SDL_EVENT_WINDOW_RESIZED:
        game_window->update_size();
        break;

    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP:
        mouse.recheck(event.button);
        break;

    case SDL_EVENT_KEY_DOWN:
    case SDL_EVENT_KEY_UP:
        keyboard.recheck(event.key);
    }
}

void game::run_processes()
{
    // Reset just_pressed-s
    keyboard.reset();

    view_events();

    // Update mouse position
    float x,y;
    SDL_GetMouseState(&x,&y);
    mouse.position = pos(x,y) + game_window->get_center() - game_window->get_half_size();

    // Update delta
    delta = (long double)(SDL_GetTicksNS() - total_delay) / NSPS;
    total_delay = SDL_GetTicksNS();

    process();
}

void game::run_collision()
{
    for (CollObj* collision : collisions)
    {
        collision->set_collision_info();
    }

    b2World_Step(coll_world, coll_progression, coll_iterations);

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
        collision->update_collision_info();
    }

    for (CollObj* collision : collisions)
    {
        collision->collision_process();
    }
}

void game::run_frame()
{
    game_window->prepare_screen();

    draw();
    draw_overlay();

    game_window->push_screen();
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
        root->_process();
    }

    else
    {
        std::cout << "No Root" << std::endl;
        running = false;
    }
}

void game::draw() const
{
    const pos origin = game_window->get_center() - game_window->get_half_size();
    for (unsigned char x = 0; x < std::numeric_limits<unsigned char>::max(); ++x)
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
    const pos origin = game_window->get_center() - game_window->get_half_size();
    for (unsigned char x = 0; x < std::numeric_limits<unsigned char>::max(); ++x)
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

void game::add_to_deletes(Process* who)
{
    deletes.push_back(who);
}

void game::add_to_collisions(CollObj* who)
{
    collisions.push_back(who);
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

    for (unsigned char x = depth + 1; x != depth; ++x) // Uses unsigned looping to get all
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

    for (unsigned char x = depth + 1; x != depth; ++x) // Uses unsigned looping to get all
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