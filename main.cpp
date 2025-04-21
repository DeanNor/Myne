
#include "game.hpp"
#include "drawobj.hpp"
#include "blendobj.hpp"
#include "collobj.hpp"
#include "joint.hpp"

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

class EditorObj : public BlendObj
{
REGISTER_OBJECT(EditorObj)

public:
    std::string represents;

    void set_representation(std::string new_class)
    {
        represents = new_class;
    }

    std::string get_representation()
    {
        return represents;
    }

    ARCHIVE(BlendObj, represents);
};

class Float : public EditorObj
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
};

class Dragable : public EditorObj
{
REGISTER_OBJECT(Dragable)

private:
    bool follow = false;

    bool clicked = false;

    static constexpr double CLOSE = 10.0;
    rect close_enough = {{-CLOSE,-CLOSE},{CLOSE,CLOSE}};

public:
    void process(double)
    {
        mouse_state& mse = get_current_game()->mouse;
        pos updated_pos = mse.position - size / 2.0;

        if (follow)
        {
            follow = mse.down;
            position = updated_pos;
        }

        else if (mse.down && !clicked && close_enough.is_inside(updated_pos, position))
        {
            follow = true;
        }

        clicked = mse.down;

        angle += 0.03;
    }

    ARCHIVE_INHERIT(EditorObj)
};

#include <filesystem>

int main()
{
    game gameplay;
    set_current_game(&gameplay);
    gameplay.physics = false;

    pos window_size = gameplay.game_window->size;

    Dragable* drawer;

    if (std::filesystem::exists("save.json"))
    {
        std::ifstream os("save.json");
        cereal::JSONInputArchive ar(os);

        ProcessFactory::loadFromArchive(ar, drawer);
    }

    else
    {
        drawer = new Dragable;
    }

    BLImage img(500, 1000, BLEND_FORMAT);

    BLContext ctx(img);
    ctx.clearAll();

    BLGradient linear(BLLinearGradientValues(0, 0, 0, 1000));
    linear.addStop(0.0, BLRgba32(0xFFBBDEF0));
    linear.addStop(1.0, BLRgba32(0xFF324376));

    ctx.fillRect(
    BLRect(0, 0, 500, 1000), linear);
    ctx.end();

    drawer->set_image(img);
    drawer->set_size({40,40});

    gameplay.root->add_child(drawer);

    gameplay.start();
    
    Uint64 tim = SDL_GetTicksNS();
    {
        std::ofstream os("save.json");
        cereal::JSONOutputArchive ar(os);

        ProcessFactory::saveToArchive(ar, drawer);
    }
    std::cout << SDL_GetTicksNS() - tim << std::endl;

    return 0;
}