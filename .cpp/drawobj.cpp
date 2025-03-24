
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

    std::vector<DrawObj*>& draws = get_current_game()->draws;

    std::vector<DrawObj*>::iterator index = std::find(draws.begin(), draws.end(), this);

    if (index != draws.end())
    {
        draws.erase(index);
    }

    else
    {
        std::cout << "HUH Draws" << std::endl;// Error, object seems already deleted and has no draw calls
    }
}

void DrawObj::draw()
{
    if (sprite != nullptr)
    {
        SDL_FRect pos_rect = pos::Make_SDL_FRect(global_position, center);

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