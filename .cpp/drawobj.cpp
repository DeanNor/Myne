
#include ".hpp/drawobj.hpp"

#include ".hpp/SDL3.h"
#include ".hpp/err.hpp"
#include ".hpp/game.hpp"
#include "SDL3/SDL_render.h"

DrawObj::DrawObj()
{
    renderer = get_current_game()->get_game_window()->get_renderer();
}

DrawObj::~DrawObj()
{
    if (sprite_ownership == true && sprite != nullptr)
    {
        SDL_DestroyTexture(sprite);
    }

    if (has_target) draw_target->remove_from_draws(this);
    
    else get_current_game()->remove_from_draws(this, depth);
}

void DrawObj::load(Loader* ar)
{
    Object::load(ar);

    sprite_ownership = ar->load_data<bool>();

    if (sprite_ownership)
    {
        sprite_path = ar->load_complex<std::string>();
        sprite_scale_mode = ar->load_data<SDL_ScaleMode>();

        set_sprite(sprite_path, sprite_scale_mode);
    }

    depth = ar->load_data<unsigned char>();
}

void DrawObj::save(Saver* ar) const
{
    Object::save(ar);

    ar->save_data(sprite_ownership);

    if (sprite_ownership)
    {
        ar->save_complex(sprite_path);
        ar->save_data(sprite_scale_mode);
    }

    ar->save_data(depth);
}

void DrawObj::draw(const pos& origin)
{
    if (sprite != nullptr && visible())
    {
        const SDL_FRect pos_rect = pos::Make_SDL_FRect(global_position.transform - origin, half_size); // Transform can be used as visible() uses compute()

        SDL_RenderTextureRotated(renderer, sprite, nullptr, &pos_rect, global_position.transform_angle.deg(), nullptr, SDL_FLIP_NONE);
    }
}

void DrawObj::draw_diva(const pos& origin)
{
    if (sprite != nullptr && visible())
    {
        const SDL_FRect pos_rect = pos::Make_SDL_FRect(global_position.transform - origin, half_size); // Transform can be used as visible() uses compute()
        std::cout << pos_rect.x << ' ' << pos_rect.y << ' ' << pos_rect.w << ' ' << pos_rect.h << std::endl;
        SDL_RenderTextureRotated(renderer, sprite, nullptr, &pos_rect, global_position.transform_angle.deg(), nullptr, SDL_FLIP_NONE);
    }
}

void DrawObj::set_sprite(SDL_Texture* bitmap, bool owns_sprite)
{
    sprite = bitmap;
    sprite_ownership = owns_sprite;

    float x,y;

    SDL_GetTextureSize(sprite, &x, &y);

    size = {x,y};
    half_size = size / 2;
}

void DrawObj::set_sprite(std::filesystem::path path, SDL_ScaleMode scale_mode)
{
    ASSERT(std::filesystem::exists(path), std::string("File path does not exist ") + path.generic_string());

    load_img(sprite, renderer, path, scale_mode);
    sprite_ownership = true;
    sprite_scale_mode = scale_mode;

    set_sprite_path(path.generic_string());
}

SDL_Texture* DrawObj::get_sprite()
{
    return sprite;
}

void DrawObj::set_sprite_path(std::string path)
{
    sprite_path = path;
}

std::string DrawObj::get_sprite_path()
{
    return sprite_path;
}

void DrawObj::set_depth(unsigned char z)
{
    depth = z;
}

unsigned char DrawObj::get_depth()
{
    return depth;
}

void DrawObj::init()
{
    has_target = false;
    window = get_current_game()->get_game_window();

    get_current_game()->add_to_draws(this, depth);
}

void DrawObj::init(unsigned char z)
{
    has_target = false;
    window = get_current_game()->get_game_window();

    depth = z;

    get_current_game()->add_to_draws(this, depth);
}

void DrawObj::init(DrawTarget* target)
{
    has_target = true;
    draw_target = target;

    target->add_to_draws(this);
}

bool DrawObj::visible()
{
    pos top_left;
    pos bottom_right;

    pos glo_pos = global_position.compute();
    rad glo_angle = global_position.compute_angle();

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

    pos rotated = half_size.rotated(glo_angle);

    pos a = glo_pos + rotated;
    pos b = glo_pos - rotated;
    pos c = pos{glo_pos.x + half_size.x, glo_pos.y - rotated.y};
    pos d = pos{glo_pos.x - half_size.x, glo_pos.y + rotated.y};

    double x_min = std::min({a.x, b.x, c.x, d.x});
    double x_max = std::max({a.x,b.x,c.x,d.x});

    double y_min = std::min({a.y, b.y, c.y, d.y});
    double y_max = std::max({a.y,b.y,c.y,d.y});

    return (x_max >= top_left.x and bottom_right.x >= x_min) and (y_max >= top_left.y and bottom_right.y >= y_min);
}

bool DrawObj::fully_visible()
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

    pos glo_pos = global_position.compute();
    rad glo_angle = global_position.compute_angle();

    pos bottom_right = (glo_pos + size).rotated(glo_angle);
    if (!bottom_right.within(window_zero, window_max))
    {
        return false;
    }

    pos top_left = (glo_pos - size).rotated(glo_angle);
    if (!top_left.within(window_zero, window_max))
    {
        return false;
    }

    pos top_right = pos(glo_pos.x + size.x, glo_pos.y - size.x).rotated(glo_angle);
    if (!top_right.within(window_zero, window_max))
    {
        return false;
    }

    pos bottom_left = pos(glo_pos.x - size.x, glo_pos.y + size.y).rotated(glo_angle);
    if (!bottom_left.within(window_zero, window_max))
    {
        return false;
    }

    return true;
}