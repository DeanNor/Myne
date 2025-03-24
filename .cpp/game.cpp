
#include "game.hpp"

#include "process.hpp"
#include "drawobj.hpp"
#include "collobj.hpp"

game::game()
{
    bool error = false;

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cout << "HUH INIT FAIL?" << std::endl;
        // ? If this happens, blow up your computer
    }

    game_window = new display({1000, 500}, "HUH", SDL_WINDOW_RESIZABLE);

    root = new Process;

    std::cout.setf(std::ios::fixed | std::ios::showpoint);
    std::cout.precision(10);
}

game::~game()
{
    if (game_window != nullptr)
    {
        delete game_window;
        game_window = nullptr;
    }
}

bool game::frame()
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
        {
            return false;
        }
    }

    Uint64 ticks = SDL_GetTicks() - total_ticks;
    if (ticks < fpsticks)
    {
        //Wait remaining time
        SDL_Delay(fpsticks - ticks);
    }
    total_ticks = SDL_GetTicks();
    
    //                                      Uint64 tim = SDL_GetTicksNS();
    process();
    
    collision_process();

    game_window->prepare_screen();
    draw();
    game_window->push_screen();

    end_delete();
    //                                      std::cout << 1 / ((double)(SDL_GetTicksNS() - tim) / 1000000000) << std::endl; // Potential FPS

    return running;
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
        root->process(spf);
    }

    else
    {
        std::cout << "No Root" << std::endl;
        running = false;
    }
}

void game::collision_process()
{
    b2World_Step(coll_world, spf, coll_iterations);

    b2SensorEvents sensors = b2World_GetSensorEvents(coll_world);

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

    b2ContactEvents contacts = b2World_GetContactEvents(coll_world);

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

void game::draw()
{
    for (DrawObj* object : draws)
    {
        object->draw();
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