
#include ".hpp/factory.hpp"
#include ".hpp/game.hpp"

#include ".hpp/process.hpp"
// #include "ast/ast_load.hpp"
// #include "ast/ast_stuff.hpp"
#include "collobjs.hpp"

#include "editorstuff.hpp"

static double fps = 1 / 60.0;

class ENDLESS : public Process
{
ASSIGN_CONSTRUCTOR(ENDLESS);

    int remaining_boxes = 8 * 3 + 4;

public:
    EditorObj* editor_var;

    void process() override
    {
        if (remaining_boxes > 0)
        {
            if (get_current_game()->get_delta() >= fps)
            {
                --remaining_boxes;
                
                EditorObj* p_editor_var = editor_var;

                editor_var = new EditorObj(hash("Object").value);
                editor_var->set_sprite(EDITOR::basic_positional, false);
                editor_var->set_size(pos(EDITOR::basic_positional->w / 2.0,EDITOR::basic_positional->h / 2.0));
                editor_var->set_position({10,10});

                editor_var->init();

                p_editor_var->add_child(editor_var);
            }
        }
    }

    
};

// #include "ast/ast.hpp"
// #include <thread>

int main()
{
    editor gameplay("HI", SDL_WINDOW_RESIZABLE, {500,500});
    set_editor(&gameplay);
    set_current_game(&gameplay);

    // std::thread ast_thread(run_prgm);
    // ast_thread.join();

    EDITOR::setup_namespace();

    EditorObj* editor_root = new EditorObj(hash("Object").value);

    gameplay.set_current_selection(editor_root);
    gameplay.set_editor_root(editor_root);

    editor_root->set_sprite(EDITOR::basic_positional, false);
    editor_root->set_size(pos(EDITOR::basic_positional->w / 2.0,EDITOR::basic_positional->h / 2.0));
    editor_root->set_position({100,100});

    editor_root->init();

    gameplay.set_physics(false);

    ENDLESS* end = new ENDLESS;
    end->add_child(editor_root);
    end->editor_var = editor_root;

    gameplay.get_root()->add_child(end);

    gameplay.start();

    gameplay.get_root()->del();

    return 0;
}