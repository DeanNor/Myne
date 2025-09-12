
#include ".hpp/drawobj.hpp"

#include ".hpp/SDL3.h"
#include ".hpp/err.hpp"
#include ".hpp/game.hpp"

#include ".hpp/ranges.hpp"

DrawObj::DrawObj()
{
    window = get_current_game()->get_game_window();
}

DrawObj::~DrawObj()
{
    if (sprite_ownership == true && sprite != nullptr)
    {
        SDL_DestroyTexture(sprite);
    }

    get_current_game()->remove_from_draws(this, depth);
}

void DrawObj::load(Loader* ar)
{
    Object::load(ar);

    scale = ar->load_complex<pos>();
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

    ar->save_complex(scale);
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

        SDL_RenderTextureRotated(window->get_renderer(), sprite, nullptr, &pos_rect, global_position.compute_angle().deg(), nullptr, SDL_FLIP_NONE);
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

    load_img(sprite, window->get_renderer(), path, scale_mode);
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
    get_current_game()->add_to_draws(this, depth);
}

void DrawObj::init(unsigned char z)
{
    depth = z;

    get_current_game()->add_to_draws(this, depth);
}

bool DrawObj::visible()
{
    pos top_left = window->get_top_left();
    pos bottom_right = window->get_bottom_right();

    pos glo_pos = global_position.compute();
    rad glo_angle = global_position.compute_angle();

    pos rotated = half_size.rotated(glo_angle);

    pos a = glo_pos + rotated;
    pos b = glo_pos - rotated;
    pos c = pos{glo_pos.x + half_size.x, glo_pos.y - rotated.y};
    pos d = pos{glo_pos.x - half_size.x, glo_pos.y + rotated.y};

    double x_min = get_lowest(4, a.x, b.x, c.x, d.x);
    double x_max = get_largest(4, a.x,b.x,c.x,d.x);

    double y_min = get_lowest(4, a.y, b.y, c.y, d.y);
    double y_max = get_largest(4, a.y,b.y,c.y,d.y);

    return (x_max >= top_left.x and bottom_right.x >= x_min) and (y_max >= top_left.y and bottom_right.y >= y_min);
}

bool DrawObj::fully_visible()
{
    const pos window_zero = window->get_top_left();
    const pos window_max = window->get_bottom_right();

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