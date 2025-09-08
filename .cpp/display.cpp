
#include ".hpp/display.hpp"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"

display::display(pos display_size, const char* name, SDL_WindowFlags flags)
{
    size = display_size;
    center = size / 2.0;
    half_size = size / 2.0;

    window = SDL_CreateWindow(name, size.x, size.y, flags);
    renderer = SDL_CreateRenderer(window, nullptr);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_SetRenderVSync(renderer, false);
}

display::~display()
{
    SDL_DestroyTexture(screen);

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
}

void display::prepare_screen()
{
    SDL_SetRenderTarget(renderer, screen);
    SDL_RenderClear(renderer);
}

void display::push_screen()
{
    if (screen)
    {
        SDL_SetRenderTarget(renderer, nullptr);

        SDL_RenderTexture(renderer, screen, nullptr, nullptr);
    }
    
    SDL_RenderPresent(renderer);
}