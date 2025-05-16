
#include "drawobj.hpp"

DrawObj::DrawObj()
{
    game* current_game = get_current_game();
    window = current_game->game_window;

    current_game->draws.push_back(this);
}

DrawObj::~DrawObj()
{
    if (ownership == true && sprite != nullptr)
    {
        SDL_DestroyTexture(sprite);
    }

    get_current_game()->remove_from_draws(this);
}

void DrawObj::draw(pos origin)
{
    if (sprite != nullptr)
    {
        SDL_FRect pos_rect = pos::Make_SDL_FRect(global_position.compute() - origin, center);

        SDL_RenderTextureRotated(window->renderer, sprite, nullptr, &pos_rect, global_position.compute_angle().deg(), nullptr, SDL_FLIP_NONE);
    }
}

void DrawObj::set_sprite(SDL_Texture* bitmap, bool owns_sprite)
{
    sprite = bitmap;
    ownership = owns_sprite;

    float x,y;

    SDL_GetTextureSize(sprite, &x, &y);

    center.x = x / 2.0;
    center.y = y / 2.0;
}

void DrawObj::set_sprite(std::string path)
{
    SDL_Surface* surf = IMG_Load(path.c_str());

    SDL_Texture* bmp = SDL_CreateTextureFromSurface(window->renderer, surf);
    set_sprite(bmp, true);

    SDL_DestroySurface(surf);
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

void DrawObj::set_display(display* new_display)
{
    window = new_display;
}

display* DrawObj::get_display()
{
    return window;
}

bool DrawObj::visible()
{
    pos window_zero = window->center - window->half_size;
    pos window_max = window->center + window->half_size;

    pos glo_pos = global_position;
    if (glo_pos.within(window_zero, window_max))
    {
        return true;
    }

    if (glo_pos.x - center.x < window_zero.x || glo_pos.x + center.x > window_max.x)
    {
        return false;
    }

    if (glo_pos.y - center.y < window_zero.y || glo_pos.y + center.y > window_max.y)
    {
        return false;
    }

    return true;
}

bool DrawObj::fully_visible()
{
    pos window_zero = window->center - window->half_size;
    pos window_max = window->center + window->half_size;

    pos glo_pos = global_position;
    if (glo_pos.x + center.x < window_zero.x || glo_pos.x - center.x > window_max.x)
    {
        return false;
    }

    if (glo_pos.y + center.y < window_zero.y || glo_pos.y - center.y > window_max.y)
    {
        return false;
    }

    return true;
}
