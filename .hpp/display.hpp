
#pragma once

#include <SDL3/SDL.h>

#include <string>
#include <iostream>

#include "pos.hpp"

struct display
{
    SDL_Window* window = nullptr;

    SDL_Renderer* renderer = nullptr;

    pos center;
    pos size;
    pos half_size;

    display(pos display_size, const char* name, SDL_WindowFlags flags);

    ~display();

    void prepare_screen();

    void push_screen();

    void update_size();
};