
#pragma once

#include ".hpp/display.hpp"
#include ".hpp/game.hpp"
#include ".hpp/object.hpp"

class Camera : public Process
{
    Object* follow;

    display* window;

    const double dampening = 0.03;

public:
    Camera()
    {
        window = get_current_game()->get_game_window();
    }

    void set_follow(Object* follower)
    {
        follow = follower;
    }

    Object* get_follow()
    {
        return follow;
    }

    virtual void process()
    {
        pos past = window->get_center();
        pos current = follow->get_position();

        pos new_center = (current - past) * dampening + past;

        if (new_center.x < (-10000 + 1000) / 2.)
        {
            new_center.x = (-10000 + 1000) / 2.;
        }

        else if (new_center.x > (10000 - 1000) / 2.)
        {
            new_center.x = (10000 - 1000) / 2.;
        }
        
        if (new_center.y < (-10000 + 1000) / 2.)
        {
            new_center.y = (-10000 + 1000) / 2.;
        }

        else if (new_center.y > (10000 - 1000) / 2.)
        {
            new_center.y = (10000 - 1000) / 2.;
        }

        window->set_center(new_center);
    }
};