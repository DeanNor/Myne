
#pragma once

#include "SDL3/SDL_pixels.h"
#include "blend2d.h"

// For some reason, the formats are backwards, what is 0xaarrggbb on blend2d is bbggrraa on SDL!!!!
constexpr BLFormat BLEND_FORMAT = BL_FORMAT_PRGB32;
constexpr SDL_PixelFormat SDL_FORMAT = SDL_PIXELFORMAT_BGRA32;