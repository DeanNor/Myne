
#pragma once

#include ".hpp/drawobj.hpp"

class DrawTarget : public DrawObj
{
protected:
    std::vector<DrawObj*> drawers;

    // Background draw color
    unsigned char r = 0,g = 0,b = 0,a = 0;

    pos origin = {0,0};

public:
    virtual void draw(const pos& global_origin) override;

    void add_to_draws(DrawObj* who)
    {
        drawers.push_back(who);
    }

    void remove_from_draws(DrawObj* who)
    {
        auto index = std::find(drawers.begin(), drawers.end(), who);

        if (index == drawers.end()) return;

        drawers.erase(index);
    }

    void set_origin(pos new_origin)
    {
        origin = new_origin;
    }

    pos get_origin() const
    {
        return origin;
    }

    pos get_zero()
    {
        return origin - half_size + global_transform.compute();
    }

    pos get_max()
    {
        return origin + half_size + global_transform.compute();
    }

    void set_rgba(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a)
    {
        r = _r;
        g = _g;
        b = _b;
        a = _a;
    }

    unsigned char get_r()
    {
        return r;
    }

    unsigned char get_g()
    {
        return g;
    }

    unsigned char get_b()
    {
        return b;
    }

    unsigned char get_a()
    {
        return a;
    }
};