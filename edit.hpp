
#pragma once

#include ".hpp/pos.hpp"

#include ".hpp/game.hpp"
#include ".hpp/SDL3.h"

namespace EDIT
{
    inline SDL_Texture* basic_positional = nullptr;

    inline pos positional_half_size;

    inline void setup_namespace()
    {
        basic_positional = load_img(get_current_game()->get_game_window()->get_renderer(), "img/default.bmp", SDL_SCALEMODE_PIXELART);

        positional_half_size = pos(basic_positional->w, basic_positional->h) / 2.;
    }
}