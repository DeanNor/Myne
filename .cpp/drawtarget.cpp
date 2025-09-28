
#include ".hpp/drawtarget.hpp"
#include "SDL3/SDL_render.h"

void DrawTarget::draw(const pos& global_origin)
{
    SDL_Texture* global_draw_target = SDL_GetRenderTarget(renderer);
    SDL_SetRenderTarget(renderer, sprite);
    
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderClear(renderer);

    for (DrawObj* drawer : drawers)
    {
        drawer->draw(global_transform.compute() + origin - half_size);
    }

    SDL_SetRenderTarget(renderer, global_draw_target);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    DrawObj::draw(global_origin);
}