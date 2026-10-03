
#pragma once

#include ".hpp/pos.hpp"

#include ".hpp/game.hpp"
#include ".hpp/SDL3.h"
#include ".hpp/sprite.hpp"

namespace EDIT
{
    extern sprite* basic_positional;
    extern sprite* phys_positional;
    extern sprite* phys_selected;
    extern pos positional_half_size;

    enum class DRAG_TYPE
    {
        NONE = 0,
        PHYS,
        NEW_PHYS
    };

    inline sprite* hull_point;

    inline void setup_namespace()
    {
        basic_positional = new sprite("img/default.tiff", get_current_game()->get_game_window()->get_renderer());
        phys_positional = new sprite("img/phys.tiff", get_current_game()->get_game_window()->get_renderer());
        phys_selected = new sprite("img/default.tiff", get_current_game()->get_game_window()->get_renderer());

        positional_half_size = pos(basic_positional->get()->w, basic_positional->get()->h) / 2.;

        hull_point = new sprite("img/default.tiff", get_current_game()->get_game_window()->get_renderer());
    }

    inline basic_sprite* get_sprite_from_type(DRAG_TYPE type)
    {
        switch (type)
        {
        case DRAG_TYPE::NONE:
            return basic_positional;
            break;
        case DRAG_TYPE::PHYS:
            return phys_positional;
            break;
        case DRAG_TYPE::NEW_PHYS:
            return phys_selected;
            break;
        }
    }
}