
#pragma once

#include <blend2d.h>

#include "object.hpp"

const BLFormat BLEND_FORMAT = BL_FORMAT_PRGB32;
const SDL_PixelFormat SDL_FORMAT = SDL_PIXELFORMAT_ARGB32;

class BlendObj : public Object
{
protected:
    BLImage image;

    SDL_Texture* texture = nullptr;

    display* window = nullptr;

    pos size;

public:
    BlendObj();

    ~BlendObj();

    virtual void draw_overlay();

    void update_sprite();

    void set_image(BLImage new_image);

    BLImage get_image();
};