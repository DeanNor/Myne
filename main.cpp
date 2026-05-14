
#include ".hpp/game.hpp"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_timer.h"
#include "editormanager.hpp"
#include "editorstuff.hpp"
#include "SDL3/SDL_video.h"
#include <cstddef>
#include <thread>
#include "ast/ast.hpp"

class Doer
{
    int v = 100;

    public:
    void doer()
    {
        std::cout << v << std::endl;
    }
};

int main()
{
    for (int x = 0; x < 10; x++)
    {
        void* c, *p;
        SDL_CreateWindowAndRenderer("Minecraft", 100, 100, 0, (SDL_Window**)&c, (SDL_Renderer**)&p);
    }

    // SDL_Delay(4000);

    // for (size_t x = 0; x < 100; x++)
    // {
    //     Doer* v = (Doer*)x;

    //     v->doer();
    // }



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