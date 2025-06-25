
#pragma once

#include <blend2d.h>

#include "SDL3/SDL_pixels.h"

// For some reason, the formats are backwards, what is 0xaarrggbb on blend2d is bbggrraa on SDL!!!!
const BLFormat BLEND_FORMAT = BL_FORMAT_PRGB32;
const SDL_PixelFormat SDL_FORMAT = SDL_PIXELFORMAT_BGRA32;