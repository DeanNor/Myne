
#include ".hpp/b2.h"
#include ".hpp/game.hpp"
#include ".hpp/process.hpp"
#include "SDL3/SDL_timer.h"
#include "SDL3/SDL_video.h"
#include "box2d/box2d.h"
#include "box2d/id.h"
#include "gambling_simulator/mainmenu.hpp"
#include "gambling_simulator/sprite_extra.hpp"

int main()
{
    // editor gameplay("HI", SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN, {1500,800});
    // set_editor(&gameplay);
    // set_current_game(&gameplay);

    // gameplay.set_physics(false);

    // EditorManager* manager = new EditorManager;
    // gameplay.set_editor_manager(manager); // No need to add child of root.

    // std::thread ast_thread(run_prgm);
    // ast_thread.join();

    // EDIT::setup_namespace();

    // gameplay.set_root(new Process);

    // std::ifstream ifile("ofile.txt");
    // if (ifile.good())
    // {
    //     json ison;
    //     ifile >> ison;
    //     auto v = find_loaded_process(ison[0]);
        
    //     gameplay.set_editor_root(v);
    //     manager->add_child(v);
    // }

    // gameplay.start();

    // json v;
    
    // if (gameplay.get_editor_root())
    // {
    //     gameplay.get_editor_root()->expansion->save_readable(v);

    //     std::ofstream file("ofile.txt");

    //     file << v.dump();
    // }

    // gameplay.get_root()->del();

    init_healthbar();

    game gameplay("Minecraft", SDL_WINDOW_RESIZABLE /*;>*/, {1000,1000});
    set_current_game(&gameplay);

    gameplay.set_physics(true);

    b2Init();

    b2WorldDef coll_world = WorldDef({0, 20});
    b2WorldId coll_id = b2CreateWorld(&coll_world);
    gameplay.set_coll_world(coll_id);
    set_current_coll_world(coll_id);

    srand(SDL_GetTicksNS());

    MainMenu* menu = new MainMenu;

    gameplay.set_root(menu);

    menu->set_depth(0);

    gameplay.start();

    gameplay.get_root()->del();

    delete SPR::lance_mist;
    delete SPR::bullet_sprite;
    delete SPR::missile_sprite;
    delete SPR::portal_particle;
    delete SPR::splash;
    
    return 0;
}