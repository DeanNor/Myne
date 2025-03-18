
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

    game_window = new display({500, 500}, "HUH", SDL_WINDOW_RESIZABLE);

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
    
    process();
    
    collision_process();

    game_window->prepare_screen();
    draw();
    game_window->push_screen();

    end_delete();

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
    coll_world->Step(spf,velocity_iterations,position_iterations);

    for (CollObj* collision : collisions)
    {
        collision->collision_process(spf);
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

b2World* coll_world = nullptr;
game* gameplay = nullptr;

void set_current_coll_world(b2World* world)
{
    coll_world = world;
}

b2World* get_current_coll_world()
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