
#include ".hpp/blendobj.hpp"

#include ".hpp/blend.h"
#include ".hpp/game.hpp"

BlendObj::BlendObj()
{
    window = get_current_game()->get_game_window();
}

BlendObj::~BlendObj()
{
    if (texture != nullptr)
    {
        SDL_DestroyTexture(texture);
    }

    get_current_game()->remove_from_overlay_draws(this, depth);
}

#include <iostream>

void BlendObj::draw_overlay(pos origin)
{
    if (texture != nullptr)
    {
        const pos glo_pos = global_position.compute() - origin;
        const SDL_FRect pos_rect{(float)glo_pos.x,(float)glo_pos.y, (float)size.x, (float)size.y};

        std::cout << pos_rect.x << ' ' << pos_rect.y << std::endl;

        SDL_RenderTextureRotated(window->get_renderer(), texture, nullptr, &pos_rect, global_position.compute_angle().deg(), nullptr, SDL_FLIP_NONE);
    }
}

void BlendObj::update_image()
{
    BLImageData data;
    image.getData(&data);

    SDL_UpdateTexture(texture, NULL, data.pixelData, data.stride);
}

void BlendObj::init()
{
    get_current_game()->add_to_overlay_draws(this, depth);
}

void BlendObj::init(unsigned char z)
{
    depth = z;

    get_current_game()->add_to_overlay_draws(this, depth);
}

void BlendObj::set_image(BLImage new_image)
{
    image = new_image;

    image_size.x = image.width();
    image_size.y = image.height();

    texture = SDL_CreateTexture(window->get_renderer(), SDL_FORMAT, SDL_TEXTUREACCESS_STREAMING, image_size.x, image_size.y);

    update_image();
}

BLImage BlendObj::get_image()
{
    return image;
}

SDL_Texture* BlendObj::get_texture()
{
    return texture;
}

void BlendObj::set_size(pos new_size)
{
    size = new_size;
}

pos BlendObj::get_size()
{
    return size;
}

void BlendObj::set_depth(unsigned char z)
{
    depth = z;
}

unsigned char BlendObj::get_depth()
{
    return depth;
}
