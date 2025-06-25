
#include "SDL3.h"
#include "game.hpp"
#include "drawobj.hpp"

#include "collobjs.hpp"
#include "process.hpp"
#include <type_traits>

SDL_Texture* text1;
SDL_Texture* text2;

class Spawner : public Process
{
    double timer = 0.02;

public:
    void process(double delta)
    {
        if (timer <= 0)
        {
            DynamObj* a = DynamObj::create();
            add_child(a);
            a->set_position({400,-40});

            DrawObj* da = new DrawObj;
            da->set_sprite(text1,false);
            a->add_child(da);

            timer = 0.00001;
        }

        else {
            timer -= delta;
        }
    }

};

int main()
{
    game gameplay("HI", SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN);
    //set_editor(&gameplay);
    set_current_game(&gameplay);

    //EDITOR::setup_namespace();

    b2Init();

    b2WorldDef world_def = WorldDef({0.,15.});
    b2WorldId coll_world = b2CreateWorld(&world_def);

    set_current_coll_world(coll_world);

    StaticObj* static_object = new StaticObj;
    gameplay.root->add_child(static_object);

    gameplay.coll_world = coll_world;

    load_img(text1, gameplay.game_window->renderer, "img/track1.png");
    load_img(text2, gameplay.game_window->renderer, "img/track2.png");

    gameplay.root->add_child(new Spawner);

    gameplay.start();

    Saver s("save.sve");
    s.save_process(gameplay.root);

    gameplay.root->del();

    return 0;
}