
#pragma once

#include "SDL3/SDL_error.h"
#include <iostream>
#include <stdexcept>

/* TODO Error window */
inline void ASSERT(bool truthy, std::string err)
{
    if ((truthy) == false)
    {
        // TODO create error window
        std::cerr << "That sucks: " << err << std::endl;

        std::cout << "SDL: " << SDL_GetError() << std::endl;

        throw std::invalid_argument(err);
    }
}