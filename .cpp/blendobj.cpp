
#include "blendobj.hpp"

BlendObj::BlendObj()
{
    game* current_game = get_current_game();
    window = current_game->game_window;

    current_game->overlay_draws.push_back(this);
}

BlendObj::~BlendObj()
{
    if (texture != nullptr)
    {
        SDL_DestroyTexture(texture);
    }

    get_current_game()->remove_from_overlay_draws(this);
}

void BlendObj::draw_overlay(pos origin)
{
    if (texture != nullptr)
    {
        pos glo_pos = global_position.compute() - origin;
        SDL_FRect pos_rect{(float)glo_pos.x,(float)glo_pos.y, (float)size.x, (float)size.y};

        SDL_RenderTextureRotated(window->renderer, texture, nullptr, &pos_rect, global_position.compute_angle().deg(), nullptr, SDL_FLIP_NONE);
    }
}

void BlendObj::update_image()
{
    BLImageData data;
    image.getData(&data);

    SDL_UpdateTexture(texture, NULL, data.pixelData, data.stride);
}

void BlendObj::set_image(BLImage new_image)
{
    image = new_image;

    image_size.x = image.width();
    image_size.y = image.height();

    texture = SDL_CreateTexture(window->renderer, SDL_FORMAT, SDL_TEXTUREACCESS_STREAMING, image_size.x, image_size.y);

    update_image();
}

BLImage BlendObj::get_image()
{
    return image;
}

void BlendObj::set_size(pos new_size)
{
    size = new_size;
}

pos BlendObj::get_size()
{
    return size;
}