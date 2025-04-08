
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

SDL_Texture* DrawObj::get_sprite()
{
    return sprite;
}

void DrawObj::set_display(display* new_display)
{
    window = new_display;
}

display* DrawObj::get_display()
{
    return window;
}