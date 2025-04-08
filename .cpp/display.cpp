
#include "display.hpp"

display::display(pos display_size, const char* name, SDL_WindowFlags flags)
{
    size = display_size;
    center = size / 2.0;
    half_size = size / 2.0;

    window = SDL_CreateWindow(name, size.x, size.y, flags);
    renderer = SDL_CreateRenderer(window, nullptr);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    //SDL_SetRenderVSync(renderer, true);
}

display::~display()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
}

void display::prepare_screen()
{
    SDL_RenderClear(renderer);
}

void display::push_screen()
{
    SDL_RenderPresent(renderer);
}

void display::update_size()
{
    int size_x, size_y;
    SDL_GetWindowSizeInPixels(window, &size_x, &size_y);
    size = {(double)size_x, (double)size_y};

    half_size = size / 2.0;
}