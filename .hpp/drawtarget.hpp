
#pragma once

#include ".hpp/drawobj.hpp"
#include ".hpp/loader.hpp"
#include ".hpp/object.hpp"
#include ".hpp/saver.hpp"

class DrawTarget : public DrawObj
{
ASSIGN_CONSTRUCTOR(DrawTarget);

protected:
    std::vector<DrawObj*> drawers; // TODO new_queue Sorry future me, im lazy

    // Background draw color // TODO rgba struct??
    unsigned char r = 0,g = 0,b = 0,a = 0;

    pos origin = {0,0};

public:
    // TODO base constructor for size?

    void save(Saver* saver) const override
    {
        Object::save(saver);

        saver->save_data(r);
        saver->save_data(g);
        saver->save_data(b);
        saver->save_data(a);
    }

    virtual void load(Loader* loader) override
    {
        Object::load(loader);

        r = loader->load_data<unsigned char>();
        g = loader->load_data<unsigned char>();
        b = loader->load_data<unsigned char>();
        a = loader->load_data<unsigned char>();
    }

    virtual void draw(const pos& global_origin, const pos& global_scale) override;

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
        return origin - texture->half_size + global_transform.compute();
    }

    pos get_max()
    {
        return origin + texture->half_size + global_transform.compute();
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