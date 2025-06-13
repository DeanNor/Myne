
#include "game.hpp"
#include "drawobj.hpp"
// #include "blendobj.hpp"
// #include "collobj.hpp"
// // #include "joint.hpp"
// #include "editorstuff.hpp"

#include "collobjs.hpp"

int main()
{
    game gameplay("HI", SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN);
    //set_editor(&gameplay);
    set_current_game(&gameplay);

    //EDITOR::setup_namespace();

    b2Init();

    b2WorldDef world_def = WorldDef({0,15});
    b2WorldId coll_world = b2CreateWorld(&world_def);

    set_current_coll_world(coll_world);

    gameplay.coll_world = coll_world;

    StaticObj* base = new StaticObj;
    gameplay.root->add_child(base);

    SDL_Texture* text;
    load_img(text, gameplay.game_window->renderer, "img/track.png");
    SDL_Texture* text2;
    load_img(text2, gameplay.game_window->renderer, "img/track2.png");

    Process* root_obj = new Process;
    for (int x = 0; x < 10; x++)
    {
        DynamObj* dyn = DynamObj::create();

        dyn->set_position({30,static_cast<double>(50 * x)});

        DynamObj* dyn2 = DynamObj::create();
        dyn->add_child(dyn2);
        dyn2->set_position({600,0});
        dyn2->set_angle(PI);

        DrawObj* drawer2 = new DrawObj;
        dyn2->add_child(drawer2);

        DrawObj* drawer = new DrawObj;
        dyn->add_child(drawer);
        drawer->set_sprite(text, false);

        drawer2->set_sprite(text2, false);

        root_obj->add_child(dyn);
    }

    gameplay.root->add_child(root_obj);

    gameplay.start();

    gameplay.root->del();

    return 0;
}