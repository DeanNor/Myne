
#include ".hpp/drawtarget.hpp"
#include "SDL3/SDL_render.h"

void DrawTarget::draw(const pos& global_origin, const pos& global_scale)
{
    SDL_Texture* global_draw_target = SDL_GetRenderTarget(renderer);
    SDL_SetRenderTarget(renderer, texture->get());
    
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderClear(renderer);

    for (DrawObj* drawer : drawers)
    {                                                               // TODO get_half_size virtual function for all types :(
        drawer->draw(global_transform.compute() + origin - ((sprite*)texture)->half_size, global_scale);
    }

    SDL_SetRenderTarget(renderer, global_draw_target);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    DrawObj::draw(global_origin, global_scale);
}