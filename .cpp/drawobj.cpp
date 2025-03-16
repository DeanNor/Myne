
#include "drawobj.hpp"

DrawObj::DrawObj()
{
    game* current_game = get_current_game();
    window = current_game->game_window;

    current_game->draws.push_back(this);
}

void DrawObj::draw()
{
    //std::cout << sprite << std::endl;
    if (sprite != nullptr)
    {
        pos computed = global_position;
        rad global_angle = global_position.compute_angle();

        SDL_FRect pos_rect = pos::make_SDL_FRect(computed, center);

        SDL_FPoint pos_point = computed;

        SDL_RenderTextureRotated(window->renderer, sprite, nullptr, &pos_rect, global_angle, &pos_point, SDL_FLIP_NONE);
    }
}

void DrawObj::set_sprite(SDL_Texture* bitmap)
{
    sprite = bitmap;

    float x,y;

    SDL_GetTextureSize(sprite, &x, &y);

    center.x = x / 2.0;
    center.y = y / 2.0;
}

SDL_Texture* DrawObj::get_sprite()
{
    return sprite;
}

void DrawObj::set_z(int new_z)
{
    z = new_z;
}

int DrawObj::get_z()
{
    return z;
}

void DrawObj::set_display(display* new_display)
{
    window = new_display;
}

display* DrawObj::get_display()
{
    return window;
}