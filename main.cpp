
#include "game.hpp"
#include "drawobj.hpp"
#include "blendobj.hpp"
#include "collobj.hpp"
#include "joint.hpp"
#include "editorstuff.hpp"

#include <iostream>
#include <fstream>

#include "collobjs.hpp"

#include <filesystem>

/* TODO Error window */
#define ASSERT(statement)\
if ((statement) == false)\
{\
    \
}\

int main()
{
    editor gameplay;
    set_editor(&gameplay);

    gameplay.physics = false;

    EditorObj* mainer = EditorObj::represent("Process");

    RepresentObj* repres = mainer->representer;

    repres->set_sprite("img/track.png");

    repres->set_size({100,100});

    gameplay.root->add_child(mainer);

    gameplay.start();

    gameplay.root->del();

    return 0;
}