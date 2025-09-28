
#include ".hpp/blendobj.hpp"

#include ".hpp/blend.h"
#include ".hpp/game.hpp"
#include "SDL3/SDL_render.h"
#include <cstring>

void BlendObj::update_image()
{
    BLImageData data;
    image.get_data(&data);

    if (sprite) SDL_DestroyTexture(sprite);
    sprite = SDL_CreateTexture(renderer, SDL_FORMAT, SDL_TEXTUREACCESS_STREAMING, size.x, size.y);

    void* values;
    int stride;
    SDL_LockTexture(sprite, nullptr, &values, &stride);
    
    values = std::memcpy(values, data.pixel_data, stride * size.y);

    SDL_UnlockTexture(sprite);

    set_size({(double)image.width(), (double)image.height()});
}

void BlendObj::set_image(BLImage new_image)
{
    image = new_image;

    image_size.x = image.width();
    image_size.y = image.height();

    update_image();
}

BLImage BlendObj::get_image()
{
    return image;
}

void BlendObj::set_size(pos new_size)
{
    size = new_size;
    
    half_size = size / 2;
}