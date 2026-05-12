
#pragma once

#include ".hpp/process.hpp"
#include "gambling_simulator/chudette.hpp"
#include "gambling_simulator/sprite_extra.hpp"

inline void WIN()
{
    get_current_game()->exit();
}

class ChudManager : public Process
{
    sprite* chudspr;

    Chuds *chuda, *chudb;

    VisibleNode *heart1, *heart2, *cover;

public:
    ChudManager(pos player_start)
    {
        chudspr = new sprite("img/player.png", get_current_game()->get_game_window()->get_renderer());

        chuda = new Chuds(chudspr, this, true);
        chuda->set_position(player_start + pos{100, 100});
        chuda->set_angle(-rad::PI());

        chudb = new Chuds(chudspr, this, false);
        chudb->set_position(player_start + pos{100, -100});
        chudb->set_angle(-rad::PI());

        chuda->set_lin_velocity({-200, 0});
        chudb->set_lin_velocity({-200, 0});

        add_child(chuda);
        add_child(chudb);

        heart1 = new VisibleNode({500, 900});
        heart1->set_scale({1/5., 1/5.});
        heart1->set_sprite("img/chud.png");
        heart1->set_depth(230);

        heart2 = new VisibleNode({500, 900});
        heart2->set_scale({1/5., 1/5.});
        heart2->set_sprite("img/foid.png");
        heart2->set_depth(230);
        
        cover = new VisibleNode({500, 900});
        cover->set_scale({1/5., 1/5.});
        cover->set_sprite("img/chud_cover.png");
        cover->set_depth(231);
    }

    ~ChudManager()
    {
        delete chudspr;
    }

    void enrage(Chuds* who)
    {
        if (who == chuda)
        {
            if (chudb)
            {
                chudb->make_enraged();
                chuda = nullptr;

                heart1->start_delete();
                cover->start_delete();
            }

            else WIN();
        }

        if (who == chudb)
        {
            if (chuda)
            {
                chuda->make_enraged();
                chudb = nullptr;

                heart2->start_delete();
                cover->start_delete();
            }

            else WIN();
        }
    }
};