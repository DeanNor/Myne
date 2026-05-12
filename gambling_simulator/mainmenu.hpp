
#pragma once

#include ".hpp/blend.h"
#include ".hpp/blendobj.hpp"
#include ".hpp/game.hpp"
#include "SDL3/SDL_render.h"
#include "wavemanager.hpp"
#include "core/font.h"
#include "core/fontface.h"
#include "core/geometry.h"
#include "core/rgba.h"

class MainMenu : public BlendObj
{
    sprite* bg_spr;

public:
    MainMenu()
    {
        BLFontFace font_face;
        font_face.create_from_file("gambling_simulator/comicSans.erererererererererer");

        BLFont text_font;
        text_font.create_from_face(font_face, 15.0f);
        
        image.create(500, 500, BLEND_FORMAT);
        BLContext ctx(image);

        ctx.set_fill_style((BLRgba32)0xFFFFFFFF);

        SDL_Renderer* renderer = get_current_game()->get_game_window()->get_renderer();
        bg_spr = new sprite("img/background.png", renderer);

        SPR::splash = new animation("img/splash", renderer);

        SPR::missile_sprite = new sprite("img/missile.png", renderer);

        SPR::missile_break = new sprite("img/missile_break.png", renderer);

        SPR::mmarker = new sprite("img/m_pointer.png", renderer);

        SPR::emarker = new sprite("img/e_pointer.png", renderer);

        ctx.clear_all();

        const BLPointI title_pos = {20,200};
        ctx.fill_utf8_text(title_pos, text_font, "Gambling Simulator");

        ctx.rotate(drad(-45));
        const BLPointI hint_pos = {20,200};
        ctx.fill_utf8_text(hint_pos, text_font, "Now with LESS crashes");
        ctx.rotate(drad(45));

        const BLPointI start_pos = {20,250};
        ctx.fill_utf8_text(start_pos, text_font, "Click to Start");

        const BLPointI legal = {90,300};
        ctx.fill_utf8_text(legal, text_font, "Legally distinct from Jet Lancer!!!");

        ctx.end();

        update_image();
    }

    virtual void process() override
    {
        BlendObj::process();

        if (get_current_game()->get_mouse().ljust_down)
        {
            start_delete();

            WaveManager* wave = new WaveManager(bg_spr);
        }
    }
};