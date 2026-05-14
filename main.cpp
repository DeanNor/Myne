
#include ".hpp/b2.h"
#include ".hpp/game.hpp"
#include "editormanager.hpp"
#include "editorstuff.hpp"
#include "SDL3/SDL_timer.h"
#include "SDL3/SDL_video.h"
#include "box2d/box2d.h"
#include "box2d/id.h"
#include <thread>
#include "ast/ast.hpp"

int main()
{
    editor gameplay("HI", SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN, {1500,800});
    set_editor(&gameplay);
    set_current_game(&gameplay);

    gameplay.set_physics(false);

    EditorManager* manager = new EditorManager;
    gameplay.set_editor_manager(manager); // No need to add child of root.

    std::thread ast_thread(run_prgm);
    ast_thread.join();

    EDIT::setup_namespace();

    gameplay.set_root(new Process);

    std::ifstream ifile("ofile.txt");
    if (ifile.good())
    {
        json ison;
        ifile >> ison;
        auto v = find_loaded_process(ison[0]);
        
        gameplay.set_editor_root(v);
        manager->add_child(v);
    }

    gameplay.start();

    json v;
    
    if (gameplay.get_editor_root())
    {
        gameplay.get_editor_root()->expansion->save_readable(v);

        std::ofstream file("ofile.txt");

        file << v.dump();
    }

    gameplay.get_root()->del();
    
    return 0;
}