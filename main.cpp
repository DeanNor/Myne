
#include ".hpp/factory.hpp"
#include ".hpp/game.hpp"

#include ".hpp/process.hpp"
#include "collobjs.hpp"

#include "editorstuff.hpp"

#include "clang_tool.hpp"

#include <thread>

static double fps = 1 / 60.0;

class ENDLESS : public Process
{
ASSIGN_CONSTRUCTOR(ENDLESS)

    EditorObj* editor_var;

public:
    ENDLESS()
    {
        editor_var = new EditorObj;
        editor_var->set_sprite(EDITOR::basic_positional, false);
        editor_var->set_size(pos(EDITOR::basic_positional->w / 2.0,EDITOR::basic_positional->h / 2.0));
        editor_var->set_position({100,100});

        editor_var->represents = std::to_string(rand());

        editor_var->init();

        add_child(editor_var);

        get_editor()->set_editor_root(editor_var);
    }

    void process() override
    {
        if (get_current_game()->delta >= fps)
        {
            EditorObj* p_editor_var = editor_var;

            editor_var = new EditorObj;
            editor_var->set_sprite(EDITOR::basic_positional, false);
            editor_var->set_size(pos(EDITOR::basic_positional->w / 2.0,EDITOR::basic_positional->h / 2.0));
            editor_var->set_position({10,10});

            editor_var->init();

            editor_var->represents = std::to_string(rand());

            p_editor_var->add_child(editor_var);
        }
    }
};

class ObjLister : public Process
{
public:
    void process()
    {
        // TODO make editorobj add to specific window via parent object
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
    //search_load("./.cpp/object.cpp");

    gameplay.root->add_child(new ENDLESS);

    gameplay.start();

    gameplay.root->del();

    search.join();

    return 0;
}