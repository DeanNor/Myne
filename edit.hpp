
#pragma once

#include ".hpp/pos.hpp"

#include ".hpp/game.hpp"
#include ".hpp/SDL3.h"
#include ".hpp/sprite.hpp"

namespace EDIT
{
    inline sprite* basic_positional;
    inline pos positional_half_size;

    inline sprite* hull_point;

    inline void setup_namespace()
    {
        basic_positional = new sprite("img/default.tiff", get_current_game()->get_game_window()->get_renderer());

        positional_half_size = pos(basic_positional->get()->w, basic_positional->get()->h) / 2.;

        hull_point = new sprite("img/default.tiff", get_current_game()->get_game_window()->get_renderer());
    }
}