
#include "display.hpp"

display::display(pos display_size, const char* name, SDL_WindowFlags flags)
{
    size = display_size;
    center = size / 2.0;
    half_size = size / 2.0;

    window = SDL_CreateWindow(name, size.x, size.y, flags);
    renderer = SDL_CreateRenderer(window, nullptr);
}

display::~display()
{
    // :( sdl does everything I want it to
}

void display::prepare_screen()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
}

void display::push_screen()
{
    SDL_RenderPresent(renderer);
}