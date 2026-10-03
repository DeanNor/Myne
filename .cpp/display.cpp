
#include "display.hpp"

#include "game.hpp"

display::display(const pos& display_size, const char* name, SDL_WindowFlags flags)
{
    size = display_size;
    half_size = size / 2.0;

    window = SDL_CreateWindow(name, size.x, size.y, flags);
    renderer = SDL_CreateRenderer(window, nullptr);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_SetRenderVSync(renderer, false);

    update_real_size();
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
    SDL_SetRenderTarget(renderer, nullptr);
    
    if (screen)
    {
        SDL_RenderTexture(renderer, screen, nullptr, nullptr);
    }
    
    SDL_RenderPresent(renderer);
}

void display::set_center(pos new_center)
{
    center = new_center;

    update_corners();

    get_current_game()->update_mouse();
}

void display::set_scale(pos new_scale)
{
    scale = new_scale;

    if (window)
    {
        compute_window_size();
    }

    else
    {
        compute_screen_size();
    }

    get_current_game()->update_mouse();
}