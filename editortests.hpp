
#include "blendobj.hpp"
#include "drawobj.hpp"

#include ".hpp/game.hpp"

struct rect
{
public:
    pos zero;
    pos max;

    bool is_inside(pos point)
    {
        return (point.x > zero.x) && (point.y > zero.y) && (point.x < max.x) && (point.y < max.y);
    }

    bool is_inside(pos point, pos offset)
    {
        return (point.x > zero.x + offset.x) && (point.y > zero.y + offset.y) && (point.x < max.x + offset.x) && (point.y < max.y + offset.y);
    }
};


class Float : public BlendObj
{
ASSIGN_CONSTRUCTOR(Float)

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

    virtual void _process(double delta) override
    {
        BlendObj::_process(delta);

        pos window_zero = window->center - window->half_size;
        position = window_zero + offset.scaled(normal, window->size);

        if (global_position.parent != nullptr)
        {
            position -= *global_position.parent->position;
        }
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

    pos get_normal()
    {
        return normal;
    }
};

class FloatScaled : public Float // Size is more of a readable value than usable, use scale instead
{
ASSIGN_CONSTRUCTOR(FloatScaled)

protected:
    pos scale;

    bool parent_scope = false;
    BlendObj* scalar_parent = nullptr;

public:
    virtual void _process(double delta) override
    {
        Float::_process(delta);

        if (parent_scope)
        {
            size = scale.scaled(normal, scalar_parent->get_size());
        }
        
        else
        {
            size = scale.scaled(normal, window->size);
        }
    }

    void set_parent(Process* new_parent) override
    {
        Float::set_parent(new_parent);

        BlendObj* par = dynamic_cast<BlendObj*>(parent);
        scalar_parent = par;

        if (scalar_parent != nullptr)
        {
            parent_scope = false;
        }
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

class Dragable : public DrawObj
{
ASSIGN_CONSTRUCTOR(Dragable)

private:
    bool follow = false;

    bool clicked = false;

    rect close_enough;

    pos past_mouse;

    pos size;

public:
    void process(double) override
    {
        mouse_state& mse = get_current_game()->mouse;
        pos updated_pos = mse.position - (size / 2.0);

        if (global_position.parent)
        {
            updated_pos -= global_position.parent->compute();
        }

        if (follow)
        {
            follow = mse.ldown;

            position += updated_pos - past_mouse;

            if (!follow)
            {
                dropped();
            }
        }

        else if (mse.ldown && !clicked && close_enough.is_inside(updated_pos, position))
        {
            follow = true;
        }

        clicked = mse.ldown;

        past_mouse = updated_pos;
    }

    virtual void dropped(){}

    void set_size(pos re_size)
    {
        size = re_size;
        close_enough = {-re_size, re_size};
    }
};