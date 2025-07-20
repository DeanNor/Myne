
#include "game.hpp"

#include "collobjs.hpp"
#include "process.hpp"

#include "editorstuff.hpp"

#include "clang_tool.hpp"

#include <thread>

static double fps = 1 / 60.0;

class ENDLESS : public Process
{
    EditorObj* editor_var;

public:
    ENDLESS()
    {
        editor_var = new EditorObj;
        editor_var->set_size(pos(EDITOR::basic_positional->w,EDITOR::basic_positional->h));
        editor_var->set_position({1,1});

        editor_var->init();

        add_child(editor_var);
    }

    void process(double delta) override
    {
        if (delta >= fps)
        {
            EditorObj* p_editor_var = editor_var;

            editor_var = new EditorObj;
            editor_var->set_size(pos(EDITOR::basic_positional->w,EDITOR::basic_positional->h));
            editor_var->set_position({1,1});

            editor_var->init();

            p_editor_var->add_child(editor_var);
        }
    }
};

int main()
{
    editor gameplay("HI", SDL_WINDOW_RESIZABLE);
    set_editor(&gameplay);
    set_current_game(&gameplay);

    gameplay.physics = false;

    EDITOR::setup_namespace();

    std::thread search(search_load,"./.cpp/object.cpp");

    gameplay.root->add_child(new ENDLESS);

    gameplay.start();

    gameplay.root->del();

    search.join();

    return 0;
}