
#include "game.h"

#include "process.h"
#include "drawobj.h"
#include "collobj.h"

game::game()
{
    bool error = false;

    if (!al_init())
    {
        error = true;
        std::cout << "Could not initalize Allegro";
    }

    if (!al_init_primitives_addon())
    {
        error = true;
        std::cout << "Could not initalize Allegro Primitives";
    }

    if (!al_init_image_addon())
    {
        error = true;
        std::cout << "Could not initalize Allegro Images";
    }

    if (!al_install_mouse())
    {
        error = true;
        std::cout << "Could not install the Mouse Drivers";
    }

    if (!error)
    {
        timer = al_create_timer(1.0 / 60.0);
        queue = al_create_event_queue();

        al_register_event_source(queue, al_get_timer_event_source(timer));

        al_start_timer(timer);

        game_window = new window(500,500, queue, "SOMETHING");
    }

    root = new Process;
}

game::~game()
{
    if (game_window != nullptr)
    {
        delete game_window;
    }

    al_destroy_timer(timer);

    al_destroy_event_queue(queue);
}

bool game::frame()
{
    ALLEGRO_EVENT event;
    al_wait_for_event(queue, &event);

    if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
    {
        return false;
    }

    if (event.type == ALLEGRO_EVENT_TIMER)
    {
        double tim = al_get_time();

        process();
        collision_process();

        al_set_target_bitmap(game_window->background);
        draw();
        al_set_target_bitmap(nullptr);

        std::cout << 1.0 / (al_get_time() - tim) << std::endl;

        game_window->push_screen();
    }

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
        root->process(0.016);
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

b2World* coll_world;
game* gameplay;

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