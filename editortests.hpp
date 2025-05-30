

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


class Float : public EditorObj
{
REGISTER_OBJECT(Float)

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

    ARCHIVE(EditorObj, offset)
};

class FloatScaled : public Float // Size is more of a readable value than usable, use scale instead
{
REGISTER_OBJECT(FloatScaled)

protected:
    pos scale;

    bool parent_scope = false;
    BlendObj* scalar_parent = nullptr;

public:
    virtual void _process(double delta)
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

    void set_parent(Process* new_parent)
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

    ARCHIVE(Float, scale)
};

class Dragable : public DrawObj
{
REGISTER_OBJECT(Dragable)

private:
    bool follow = false;

    bool clicked = false;

    rect close_enough;

    pos size;

public:
    void process(double)
    {
        mouse_state& mse = get_current_game()->mouse;
        pos updated_pos = mse.position - size / 2.0;

        if (follow)
        {
            follow = mse.ldown;

            // TODO I guess it doesnt work :<
            global_position.subt(updated_pos);

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
    }

    virtual void dropped(){}

    void set_size(pos re_size)
    {
        size = re_size;
        close_enough = {-re_size, re_size};
    }

    ARCHIVE_INHERIT(DrawObj)
};