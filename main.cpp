
#include ".hpp/drawobj.hpp"
#include ".hpp/game.hpp"

#include ".hpp/process.hpp"
#include "collobjs.hpp"

int main()
{
    game gameplay("HI", SDL_WINDOW_RESIZABLE);
    set_current_game(&gameplay);

    gameplay.physics = false;

    DrawObj* drawer = new DrawObj;

    srand(time(0));
    drawer->set_position(pos(rand() % 500, rand() % 500));

    drawer->set_sprite("sample.bmp");
    drawer->init();

    gameplay.root = drawer;  //->add_child(drawer);

    gameplay.start();

    gameplay.root->del();

    return 0;
}