
#pragma once

#include "SDL3/SDL_pixels.h"
#include <blend2d.h>

// For some reason, the formats are backwards, what is going on!!!! Mainly a Blend2d problem, but good luck fixing that
constexpr const BLFormat BLEND_FORMAT = BL_FORMAT_PRGB32;
constexpr const SDL_PixelFormat SDL_FORMAT = SDL_PIXELFORMAT_ARGB8888;