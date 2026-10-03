
#include "game.hpp"

#include "process.hpp"
#include "drawobj.hpp"
#include "collobj.hpp"

#include <cstddef>

#include "SDL3.h"
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_init.h>

#ifdef EDITOR
#include "imgui_impl_sdl3.h"

inline ImGuiIO& init_imgui()
{
    ImGui::CreateContext();
    return ImGui::GetIO();
}
#endif

game::game(const char* name, SDL_WindowFlags flags, pos window_size)
#ifdef EDITOR
    : io(init_imgui())
#endif
{
    if (!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO))
    {
        std::cout << "HUH INIT FAIL?\n";
        // TODO If this happens, blow up your computer

        std::cout << SDL_GetError() << '\n' << '\n';
    }

    game_window = new display(window_size, name, flags);
    game_window->update_size();

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

    kill_audio_streams();

    SDL_Quit();
}

bool game::frame()
{
    Uint64 tick_count = SDL_GetTicks();
    Uint64 ticks = tick_count - total_ticks;
    Uint64 coll_ticks = tick_count - total_coll_ticks;
    Uint64 frame_ticks = tick_count - total_frame_ticks;

    int count = floor(ticks / fpsticks);
    int coll_count = physics ? floor(coll_ticks / collticks) : 0;
    bool frame_count = frame_ticks >= frameticks;

    if (count > 0 || coll_count > 0 || frame_count)
    {
        do
        {
            if (count > 0)
            {
                total_ticks = tick_count;

                update_run_data();
                view_events();

                run_processes();

                kill_audio_streams();
                --count;
            }

            if (physics)
            {
                if (coll_count > 0)
                {
                    total_coll_ticks = tick_count;

                    run_collision();
                    --coll_count;
                }
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
        SDL_Delay(std::min({fpsticks - ticks, collticks - coll_ticks, frameticks - frame_ticks}));
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
#ifdef EDITOR
    ImGui_ImplSDL3_ProcessEvent(&event);
#endif

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

    case SDL_EVENT_MOUSE_BUTTON_UP:
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
#ifdef EDITOR
        if (!io.WantCaptureMouse)
        {
            mse_was_on_global = true;
#endif
            mouse.recheck(event.button);
#ifdef EDITOR
        }
#endif
        break;

    case SDL_EVENT_MOUSE_WHEEL:
#ifdef EDITOR
        if (!io.WantCaptureMouse)
        {
#endif
            mouse.scroll(event.wheel);
#ifdef EDITOR
        }
#endif

        break;
    
    case SDL_EVENT_MOUSE_MOTION:
#ifdef EDITOR
        if (!io.WantCaptureMouse)
        {
#endif
            mouse.move(event.motion);
#ifdef EDITOR
        }
#endif
        break;

    case SDL_EVENT_KEY_DOWN:
    case SDL_EVENT_KEY_UP:
#ifdef EDITOR
        if (!io.WantTextInput)
        {
#endif
            keyboard.recheck(event.key);
#ifdef EDITOR
        }
#endif
        break;
    }
}

void game::update_mouse()
{
    // Update mouse position
    float x,y;
    SDL_GetMouseState(&x,&y);
    mouse.global_position = pos(x,y);
    mouse.position = ((pos(x,y)) / game_window->get_scale()) + game_window->get_center() - game_window->get_half_size();
}

void game::update_run_data()
{
    // Reset just_pressed-s
    keyboard.reset();
    mouse.reset();

    update_mouse();

#ifdef EDITOR
    if (io.WantCaptureMouse && mse_was_on_global)
    {
        mouse.blank();

        mse_was_on_global = false;
    }
#endif
}

void game::run_processes()
{
    // Update delta
    delta = (long double)(SDL_GetTicksNS() - total_delay) / NSPS;
    total_delay = SDL_GetTicksNS();

    process();
}

void game::run_collision()
{
    collisions.reserve(new_collisions.size());
    for (auto x : new_collisions)
    {
        collisions.push_back(x);
    }
    new_collisions.clear();

    for (CollObj* collision : collisions) // Update b2 values before b2 process
    {
        collision->set_collision_info(coll_progression);
    }

    b2World_Step(coll_world, coll_progression, coll_iterations);

    for (CollObj* collision : collisions) // Update CollObj values to b2 values before process frame and events
    {
        collision->update_collision_info();

        collision->collision_process();
    }

    const b2SensorEvents sensors = b2World_GetSensorEvents(coll_world);

    for (size_t x = 0; x < sensors.beginCount; ++x)
    {
        b2SensorBeginTouchEvent* event = sensors.beginEvents + x;
        CollObj::SensorBegin(event->sensorShapeId, event->visitorShapeId);
    }

    for (size_t y = 0; y < sensors.endCount; ++y)
    {
        b2SensorEndTouchEvent* event = sensors.endEvents + y;
        CollObj::SensorEnd(event->sensorShapeId, event->visitorShapeId);
    }

    const b2ContactEvents contacts = b2World_GetContactEvents(coll_world);

    for (size_t x = 0; x < contacts.beginCount; ++x)
    {
        b2ContactBeginTouchEvent* event = contacts.beginEvents + x;
        CollObj::CollisionBegin(event->shapeIdA, event->shapeIdB);
    }

    for (size_t y = 0; y < contacts.endCount; ++y)
    {
        b2ContactEndTouchEvent* event = contacts.endEvents + y;
        CollObj::CollisionEnd(event->shapeIdA, event->shapeIdB);
    }
}

void game::run_frame()
{
    for (auto x : new_draws)
    {
        draws[x.second].push_back(x.first);
    }

    new_draws.clear();

    game_window->prepare_screen();

    draw();

    game_window->push_screen();
}

void game::start()
{
    running = true;
    while(frame());
}

void game::exit()
{
    running = false;
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
    const pos origin = (game_window->get_center() - game_window->get_half_size()) * game_window->get_scale();

    unsigned char x = 0;
    do
    {
        const std::vector<DrawObj*>& layer = draws[x];
        for (DrawObj* object : layer)
        {
            object->draw(origin, game_window->get_scale());
        }
        
        ++x;
    } while (x != 0);
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
    new_collisions.push_back(who);
}

void game::add_to_draws(DrawObj* who, const unsigned char& depth)
{
    new_draws.push_back({who, depth});
}

bool game::__remove_from_draws(DrawObj* who, const unsigned char& depth)
{
    std::vector<DrawObj*>& layer = draws[depth];
    auto index = std::find(layer.begin(), layer.end(), who);

    if (index == layer.end()) return false;

    layer.erase(index);
    return true;
}

bool game::__remove_from_new_draws(DrawObj* who, const unsigned char& depth)
{
    auto index = std::find(new_draws.begin(), new_draws.end(), {who, depth});

    if (index == new_draws.end()) return false;

    new_draws.erase(index);
    return true;
}

void game::remove_from_draws(DrawObj* who, const unsigned char& depth)
{
    if (__remove_from_draws(who,depth)) return;

    if (__remove_from_new_draws(who,depth)) return;

    for (unsigned char x = depth + 1; x != depth; ++x) // Uses unsigned looping to get all
    {
        if (__remove_from_draws(who, x)) return;
    }

    std::cout << who->get_name() << std::endl;
    std::cout << "Huh Draws" << std::endl;
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
        const std::vector<CollObj*>::iterator new_index = std::find(new_collisions.begin(), new_collisions.end(), who);

        if (index != new_collisions.end())
        {
            new_collisions.erase(new_index);
        }

        std::cout << "HUH Collision" << std::endl; // Error, object seems already deleted and has no draw calls
    }
}

void game::kill_audio_streams()
{
    std::vector<std::vector<SDL_AudioStream*>::iterator> indexes;

    for (auto x = audio_streams.begin(); x != audio_streams.end(); ++x)
    {
        if (SDL_GetAudioStreamQueued(*x) <= 0)
        {
            indexes.push_back(x);
        }
    }

    for (auto x : indexes)
    {
        audio_streams.erase(x);
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