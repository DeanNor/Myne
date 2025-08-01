
#pragma once

#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"

#include ".hpp/pos.hpp"

struct display
{
    SDL_Window* window = nullptr;

    SDL_Renderer* renderer = nullptr;

    pos center;
    pos size;
    pos half_size;

    pos original_size;

    display(pos display_size, const char* name, SDL_WindowFlags flags);

    ~display();

    void prepare_screen();

    void push_screen();

    void update_size();
};