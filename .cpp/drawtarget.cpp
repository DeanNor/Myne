
#include ".hpp/drawtarget.hpp"


void DrawTarget::draw(const pos& global_origin)
{
    SDL_Texture* global_draw_target = SDL_GetRenderTarget(renderer);
    SDL_SetRenderTarget(renderer, sprite);
    SDL_RenderClear(renderer);

    for (DrawObj* drawer : drawers)
    {
        drawer->draw(origin - half_size);
    }

    SDL_SetRenderTarget(renderer, global_draw_target);

    DrawObj::draw(global_origin);
}