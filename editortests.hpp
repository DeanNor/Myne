
#include ".hpp/blendobj.hpp"
#include ".hpp/drawobj.hpp"

#include ".hpp/game.hpp"

struct rect
{
public:
    pos zero;
    pos max;

    bool contains(pos point)
    {
        return (point.x > zero.x) && (point.y > zero.y) && (point.x < max.x) && (point.y < max.y);
    }
};


class Float : public BlendObj
{
ASSIGN_CONSTRUCTOR(Float);

protected:
    pos offset;

    pos normal;

    display* window = nullptr;

public:
    Float()
    {
        window = get_current_game()->get_game_window();
        normal = window->get_size();
    }

    virtual void _process() override
    {
        BlendObj::_process();

        pos window_zero = window->get_center() - window->get_half_size();
        position = window_zero + offset.scaled(normal, window->get_size());

        if (global_transform.parent != nullptr)
        {
            position -= *global_transform.parent->position;
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
ASSIGN_CONSTRUCTOR(FloatScaled);

protected:
    pos scale;

    bool parent_scope = false;
    BlendObj* scalar_parent = nullptr;

public:
    virtual void _process() override
    {
        Float::_process();

        if (parent_scope)
        {
            size = scale.scaled(normal, scalar_parent->get_size());
        }
        
        else
        {
            size = scale.scaled(normal, window->get_size());
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

// Due to the implementation of dragging, no children other than Dragables and child classes can be had. It also assumes that a base Dragable is the root position.
class Dragable : public DrawObj
{
ASSIGN_CONSTRUCTOR(Dragable);

private:
    rect close_enough;

    pos size;

public:
    Dragable* check_click(pos global_mouse)
    {
        pos updated_mouse = global_mouse - position;

        for (Process* child : children)
        {
            Dragable* ret_val = ((Dragable*)child)->check_click(updated_mouse);

            if (ret_val) return ret_val;
        }

        if (close_enough.contains(updated_mouse)) return this;

        return nullptr;
    }

    void set_size(pos re_size)
    {
        size = re_size;
        close_enough = {-re_size, re_size};
    }
};
