
#pragma once

#include ".hpp/drawobj.hpp"
#include ".hpp/game.hpp"
#include ".hpp/process.hpp"
#include ".hpp/sprite.hpp"
#include "gambling_simulator/chud_boss_manager.hpp"
#include "player.hpp"

class SideBar : public DrawObj
{
    bool left, done = false;

    double offset = 1000 / 6.;
    double normal = 500 - offset / 2.;

public:
    SideBar(bool _left) : left(_left)
    {
        set_depth(210);
        set_sprite("img/sidebar.png");
    }

    virtual void draw(const pos& origin, const pos& global_scale) override
    {
        if (!done)
        {
            offset -= 1;

            if (offset <= 0)
            {
                offset = 0;
                done = true;
            }
        }

        pos screen_pos = window->get_center();
        if (left) screen_pos.x -= normal + offset;
        else screen_pos.x += normal + offset;

        position = screen_pos;

        DrawObj::draw(origin, global_scale);
    }
};

class WaveManager : public Process
{
    static constexpr double delay = 0.8;
    double count = delay;

public:
    WaveManager(sprite* bg_spr)
    {
        get_current_game()->set_root(this);

        add_child(new Player);

        DrawObj* bg = new DrawObj;
        bg->set_depth(0);
        bg->set_sprite(bg_spr, true);
        add_child(bg);

        // add_child(new SideBar(true));
        // add_child(new SideBar(false));

        add_child(new ChudManager(get_player()->get_position()));
    }
};