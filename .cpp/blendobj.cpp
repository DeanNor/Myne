
#include ".hpp/blendobj.hpp"

#include ".hpp/blend.h"
#include ".hpp/game.hpp"
#include "SDL3/SDL_render.h"
#include <cstring>

void BlendObj::update_image()
{
    update_image_size();

    if (!texture || image_size != ((sprite*)texture)->size)
    {
        SDL_DestroyTexture(texture->get());

        SDL_Texture* new_texture = SDL_CreateTexture(renderer, SDL_FORMAT, SDL_TEXTUREACCESS_STREAMING, image_size.x, image_size.y);

        ((sprite*)texture)->set(new_texture);
    }

    BLImageData data;
    image.get_data(&data);

    void* values;
    int stride;
    SDL_LockTexture(texture->get(), nullptr, &values, &stride);
    
    values = std::memcpy(values, data.pixel_data, stride * image_size.y);

    SDL_UnlockTexture(texture->get());
}

void BlendObj::update_image_size()
{
    image_size.x = image.width();
    image_size.y = image.height();
}

void BlendObj::set_image(BLImage new_image)
{
    image = new_image;

    update_image();
}

BLImage BlendObj::get_image()
{
    return image;
}