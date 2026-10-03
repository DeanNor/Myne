
#include ".hpp/drawobj.hpp"

#include ".hpp/err.hpp"
#include ".hpp/game.hpp"
#include ".hpp/drawtarget.hpp"
#include ".hpp/sprite.hpp"
#include "SDL3/SDL_surface.h"

#ifdef EDITOR
#include "edit.hpp"
#endif

DrawObj::DrawObj()
{
    renderer = get_current_game()->get_game_window()->get_renderer();

    global_transform.scale = &scale;
}

DrawObj::~DrawObj()
{
    if (sprite_ownership == true && texture != nullptr)
    {
        delete texture;
    }

    if (initialized)
    {
        if (has_target) draw_target->remove_from_draws(this);
    
        else get_current_game()->remove_from_draws(this, depth);
    }
}

void DrawObj::load(Loader* ar)
{
    Object::load(ar);

    sprite_ownership = ar->load_data<bool>();

    if (sprite_ownership)
    {
        texture = ar->load_complex_ptr<sprite>();
    }

    depth = ar->load_data<unsigned char>();
}

void DrawObj::save(Saver* ar) const
{
    Object::save(ar);

    ar->save_data(sprite_ownership);

    if (sprite_ownership)
    {
        ar->save_complex_ptr(texture);
    }

    ar->save_data(depth);
}

void DrawObj::draw(const pos& origin, const pos& global_scale)
{
    if (active_drawer)
    {
        if (texture != nullptr)
        {
            texture->display(global_transform.compute() * global_scale - origin, global_transform.compute_scale() * global_scale, get_global_angle().deg(), renderer);
        }
    }
}

void DrawObj::set_sprite(basic_sprite* new_sprite, bool owns_sprite)
{
    texture = new_sprite;
    sprite_ownership = owns_sprite;
}

void DrawObj::set_sprite(std::filesystem::path path, SDL_ScaleMode scale_mode)
{
    ASSERT(std::filesystem::exists(path), std::string("File path does not exist: ") + path.generic_string());

    texture = new sprite(path, renderer);

    sprite_ownership = true;
}

basic_sprite* DrawObj::get_texture() const
{
    return texture;
}

void DrawObj::set_depth(unsigned char z)
{    
    if (initialized)
    {
        if (has_target)
        {
            draw_target->remove_from_draws(this);
        }

        else
        {
            get_current_game()->remove_from_draws(this, depth);
        }
    }

    depth = z;
        
    has_target = false;
    window = get_current_game()->get_game_window();

    get_current_game()->add_to_draws(this, depth);

    initialized = true;
}

unsigned char DrawObj::get_depth() const
{
    return depth;
}

void DrawObj::target(DrawTarget* target)
{
    has_target = true;
    draw_target = target;

    target->add_to_draws(this);

    initialized = true;
}

bool DrawObj::visible()
{
    sprite* spr = dynamic_cast<sprite*>(texture);

    if (spr)
    {
        pos top_left;
        pos bottom_right;

        pos glo_pos = global_transform.compute();
        rad glo_angle = global_transform.compute_angle();

        if (has_target)
        {
            top_left = draw_target->get_zero();
            bottom_right = draw_target->get_max();
        }

        else
        {
            top_left = window->get_top_left();
            bottom_right = window->get_bottom_right();
        }

        pos rotated = spr->half_size.rotated(glo_angle);

        pos a = glo_pos + rotated;
        pos b = glo_pos - rotated;
        pos c = pos{glo_pos.x + spr->half_size.x, glo_pos.y - rotated.y};
        pos d = pos{glo_pos.x - spr->half_size.x, glo_pos.y + rotated.y};

        double x_min = std::min({a.x, b.x, c.x, d.x});
        double x_max = std::max({a.x,b.x,c.x,d.x});

        double y_min = std::min({a.y, b.y, c.y, d.y});
        double y_max = std::max({a.y,b.y,c.y,d.y});

        return ((x_max > top_left.x and bottom_right.x > x_min) and (y_max > top_left.y and bottom_right.y > y_min));
    }

    return false;
}

bool DrawObj::fully_visible()
{
    sprite* spr = dynamic_cast<sprite*>(texture);

    if (spr)
    {
        pos window_zero;
        pos window_max;

        if (has_target)
        {
            window_zero = draw_target->get_zero();
            window_max = draw_target->get_max();
        }

        else
        {
            window_zero = window->get_top_left();
            window_max = window->get_bottom_right();
        }

        pos glo_pos = global_transform.compute();
        rad glo_angle = global_transform.compute_angle();

        pos bottom_right = (glo_pos + spr->size).rotated(glo_angle);
        if (!bottom_right.within(window_zero, window_max))
        {
            return false;
        }

        pos top_left = (glo_pos - spr->size).rotated(glo_angle);
        if (!top_left.within(window_zero, window_max))
        {
            return false;
        }

        pos top_right = pos(glo_pos.x + spr->size.x, glo_pos.y - spr->size.y).rotated(glo_angle);
        if (!top_right.within(window_zero, window_max))
        {
            return false;
        }

        pos bottom_left = pos(glo_pos.x - spr->size.x, glo_pos.y + spr->size.y).rotated(glo_angle);
        if (!bottom_left.within(window_zero, window_max))
        {
            return false;
        }

        return true;
    }

    return false;
}