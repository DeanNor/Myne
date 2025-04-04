
#pragma once

#include <blend2d.h>

#include "object.hpp"

// For some reason, the formats are backwards, what is 0xaarrggbb on blend2d is bbggrraa on SDL!!!!
const BLFormat BLEND_FORMAT = BL_FORMAT_PRGB32;
const SDL_PixelFormat SDL_FORMAT = SDL_PIXELFORMAT_BGRA32;

class BlendObj : public Object
{
protected:
    BLImage image;

    SDL_Texture* texture = nullptr;

    display* window = nullptr;

    pos image_size;
    pos size;

public:
    BlendObj();

    ~BlendObj();

    virtual void draw_overlay();

    void update_image();

    void set_image(BLImage new_image);

    BLImage get_image();

    void set_size(pos new_size);

    pos get_size();
};