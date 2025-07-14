
#include "SDL3.h"
#include "SDL3/SDL_video.h"
#include "game.hpp"
#include "drawobj.hpp"

#include "collobjs.hpp"
#include "process.hpp"
#include <chrono>
#include <random>

SDL_Texture* text1;

class Spawner : public Process
{
public:
    void process(double)
    {
        DynamObj* a = DynamObj::create();
        add_child(a);
        a->set_position({get_current_game()->game_window->center.x,50});

        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<> dis(-PI, PI);
        static std::uniform_real_distribution<float> dis2(-100.0f, 100.0f);

        a->set_angle(dis(gen));

        b2Body_SetLinearVelocity(a->get_collision_body(), {dis2(gen), dis2(gen)});

        DrawObj* da = new DrawObj;
        da->set_sprite(text1,false);
        a->add_child(da);
    }
};

int main()
{
    game gameplay("HI", SDL_WINDOW_RESIZABLE);
    //set_editor(&gameplay);
    set_current_game(&gameplay);

    //EDITOR::setup_namespace();

    b2Init();

    b2WorldDef world_def = WorldDef({0.,100.});
    b2WorldId coll_world = b2CreateWorld(&world_def);

    set_current_coll_world(coll_world);

    StaticObj* static_object = new StaticObj;
    gameplay.root->add_child(static_object);

    gameplay.coll_world = coll_world;

    load_img(text1, gameplay.game_window->renderer, "img/track1.png");

    gameplay.root->add_child(new Spawner);

    gameplay.start();

    auto tim = std::chrono::high_resolution_clock::now().time_since_epoch();
    Saver s("save.sve");
    s.save_process(gameplay.root);
    std::cout << std::chrono::high_resolution_clock::now().time_since_epoch() - tim << std::endl;

    gameplay.root->del();

    return 0;
}