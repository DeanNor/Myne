#include <iostream>
#include <vector>

#include "game.hpp"
#include "drawobj.hpp"
#include "blendobj.hpp"
#include "collobj.hpp"
#include "joint.hpp"

class Float : public BlendObj
{
protected:
    pos offset;

    pos normal;

    display* window = nullptr;

public:
    Float()
    {
        window = get_current_game()->game_window;
        normal = window->size;
    }

    virtual void _process(double delta)
    {
        BlendObj::_process(delta);

        pos window_zero = window->center - window->half_size;

        position = window_zero + offset.scaled(normal, window->size);
    }

    void set_position(pos new_position) = delete;

    void set_offset(pos new_offset)
    {
        offset = new_offset;
    }

    pos get_offset()
    {
        return offset;
    }
};

class FloatScaled : public Float // Size is more of a readable value than usable, use scale instead
{
protected:
    pos scale;

public:
    virtual void _process(double delta)
    {
        Float::_process(delta);

        size = scale.scaled(normal, window->size);
    }

    void set_normal(pos new_normal)
    {
        normal = new_normal;
    }

    pos get_normal()
    {
        return normal;
    }

    void set_size(pos) = delete;

    void set_scale(pos new_scale)
    {
        scale = new_scale;
    }

    pos get_scale()
    {
        return scale;
    }
};

int main()
{
    game gameplay;
    set_current_game(&gameplay);
    gameplay.physics = false;

    pos window_size = gameplay.game_window->size;

    FloatScaled* drawer = new FloatScaled;
    FloatScaled* drawer2 = new FloatScaled;
    drawer->set_offset({0,0});
    drawer2->set_offset({800,0});

    BLImage img(500, 1000, BLEND_FORMAT);

    BLContext ctx(img);
    ctx.clearAll();

    BLGradient linear(
    BLLinearGradientValues(0, 0, 0, 1000));
    linear.addStop(0.0, BLRgba32(0xFFBBDEF0));
    linear.addStop(1.0, BLRgba32(0xFF324376));

    ctx.fillRect(
    BLRect(0, 0, 500, 1000), linear);

    drawer->set_image(img);
    drawer->set_scale({200,500});

    drawer2->set_image(img);
    drawer2->set_scale({200,500});

    gameplay.root->add_child(drawer);
    gameplay.root->add_child(drawer2);

    

    gameplay.start();

    return 0;
}