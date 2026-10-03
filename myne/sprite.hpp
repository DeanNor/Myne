
#pragma once

#include "SDL3.h"
#include "factory.hpp"
#include "game.hpp"

#include "pos.hpp"

#include <cstddef>
#include <filesystem>
#include <vector>

struct basic_texture
{
VIR_NAME_TYPE_OVERRIDE(basic_sprite);

public:
    // Both size and half size of the texture
    pos size;
    pos half_size;

    SDL_FlipMode flip_mode = SDL_FlipMode::SDL_FLIP_NONE;

    basic_texture() = default;

    basic_texture(pos _size) : size(_size), half_size(_size / 2.)
    {

    }

    virtual ~basic_texture() = default;

    virtual void save(Saver* os) const
    {
        os->save_complex(size);
    }

    virtual void load(Loader* os)
    {
        size = os->load_complex<pos>();

        half_size = size / 2.0;
    }
};

struct basic_sprite : public basic_texture
{
VIR_NAME_TYPE(basic_sprite);

public:
    std::filesystem::path sprite_path;

    basic_sprite() = default;

    basic_sprite(std::string _sprite_path) : sprite_path(_sprite_path)
    {

    }

    basic_sprite(pos _size, std::filesystem::path _sprite_path) : basic_texture(_size), sprite_path(_sprite_path)
    {

    }

    virtual void set(SDL_Texture* v) {};
    virtual SDL_Texture* get() = 0;
    virtual void display(const pos& where, const pos& scale, double angle, SDL_Renderer* renderer) = 0;

    virtual void save(Saver* os) const override
    {
        basic_texture::save(os);

        os->save_complex<std::string>(sprite_path.string());
    }

    virtual void load(Loader* os) override
    {
        basic_texture::load(os);

        sprite_path = os->load_complex<std::string>();
    }
};

struct sprite : public basic_sprite
{
ASSIGN_VIR_VAR_CONSTRUCTOR(sprite);

public:
    SDL_Texture* texture = nullptr;

    sprite() = default;

    sprite(std::filesystem::path _sprite_path, SDL_Renderer* renderer) : basic_sprite(_sprite_path.string()), texture(load_img(renderer, _sprite_path.string()))
    {
        size = (pos)texture;
        half_size = size / 2.0;
    }

    ~sprite()
    {
        SDL_DestroyTexture(texture);
    }

    virtual SDL_Texture* get() override
    {
        return texture;
    }

    void set(SDL_Texture* v) override
    {
        texture = v;

        size = (pos)texture;
        half_size = size / 2.0;
    }

    virtual void display(const pos& where, const pos& scale, double angle, SDL_Renderer* renderer) override
    {
        const SDL_FRect frect = pos::Make_SDL_FRect(where, half_size * scale);
        SDL_RenderTextureRotated(renderer, texture, nullptr, &frect, angle, nullptr, flip_mode);
    }

    virtual void save(Saver* os) const override
    {
        basic_sprite::save(os);
    }

    virtual void load(Loader* os) override
    {
        basic_sprite::load(os);

        texture = load_img(get_current_game()->get_game_window()->get_renderer(), sprite_path);
    }
};

struct tile_position : public basic_texture
{
ASSIGN_VIR_VAR_CONSTRUCTOR(tile_position);

public:
    pos sheet_position; // Position on texture sheet

    tile_position()
    {
        // TODO
    }

    void display(const pos& where, const pos& scale, double angle, SDL_Texture* what, SDL_Renderer* renderer)
    {
        const SDL_FRect frect = pos::Make_SDL_FRect(where, half_size * scale);
        const SDL_FRect srect = {(float)sheet_position.x, (float)sheet_position.y, (float)(sheet_position.x + size.x), (float)(sheet_position.y + size.y)};
        SDL_RenderTextureRotated(renderer, what, &srect, &frect, angle, nullptr, flip_mode);
    }

    virtual void save(Saver* os) const override
    {
        basic_texture::save(os);

        os->save_complex(sheet_position);
    }

    virtual void load(Loader* os) override
    {
        basic_texture::load(os);

        sheet_position = os->load_complex<pos>();
    }
};

struct tilesheet : public basic_sprite
{
ASSIGN_VIR_VAR_CONSTRUCTOR(tilesheet);

public:
    SDL_Texture* texture = nullptr;
    std::vector<tile_position> tiles;
    std::vector<pos> positions;

    tilesheet()
    {
        // TODO
    }

    ~tilesheet()
    {
        SDL_DestroyTexture(texture);
    }
    
    virtual SDL_Texture* get() override
    {
        return texture;
    }

    virtual void display(const pos& where, const pos& scale, double angle, SDL_Renderer* renderer) override
    {
        for (size_t x = 0; x < tiles.size(); ++x)
        {
            tiles.at(x).display(where + positions.at(x), scale, angle, texture, renderer);
        }
    }

    virtual void save(Saver* os) const override
    {
        basic_sprite::save(os);

        os->save_data<size_t>(tiles.size());
        for (tile_position x : tiles)
        {
            x.save(os);
        }

        os->save_data<size_t>(positions.size());
        for (pos x : positions)
        {
            x.save(os);
        }
    }

    virtual void load(Loader* os) override
    {
        basic_sprite::load(os);

        size_t list_size = os->load_data<size_t>();
        tiles.reserve(list_size);
        for (size_t x = 0; x < list_size; ++x)
        {
            tile_position tile = os->load_complex<tile_position>();

            tiles.push_back(tile);
        }

        size_t pos_list_size = os->load_data<size_t>();
        positions.reserve(list_size);
        for (size_t x = 0; x < list_size; ++x)
        {
            pos position = os->load_complex<pos>();

            positions.push_back(position);
        }
    }
};

struct animation_sheet : public tilesheet
{
ASSIGN_VIR_VAR_CONSTRUCTOR(animation_sheet);

public:
    size_t iter = 0;

    animation_sheet()
    {
        // TODO
    }

    // get() implementation in tilesheet
    
    virtual void display(const pos& where, const pos& scale, double angle, SDL_Renderer* renderer) override
    {
        tiles.at(iter).display(where + positions.at(iter), scale, angle, texture, renderer);
        ++iter;

        if (iter >= tiles.size())
        {
            iter = 0;
        }
    }

    virtual void save(Saver* os) const override
    {
        tilesheet::save(os);

        os->save_data<size_t>(iter);
    }

    virtual void load(Loader* os) override
    {
        tilesheet::load(os);

        iter = os->load_data<size_t>();
    }
};

struct animation : public basic_sprite
{
ASSIGN_VIR_VAR_CONSTRUCTOR(animation);

public:
    std::vector<sprite*> sprites;
    size_t iter = 0;

    animation()
    {
        // TODO
    }

    animation(std::string folder, SDL_Renderer* renderer)
    {
        std::filesystem::directory_iterator dir(folder);

        for (auto x : dir)
        {
            sprites.push_back(new sprite(x.path(), renderer));
        }
    }

    ~animation()
    {
        for (auto x : sprites)
        {
            delete x;
        }
    }

    sprite* at(size_t v)
    {
        return sprites.at(v);
    }
    
    virtual SDL_Texture* get() override
    {
        return sprites.at(iter)->get();
    }

    void set_frame(std::size_t count)
    {
        iter = count;
    }

    std::size_t get_frame()
    {
        return iter;
    }

    std::size_t get_len()
    {
        return sprites.size();
    }

    virtual void display(const pos& where, const pos& scale, double angle, SDL_Renderer* renderer) override
    {
        sprites.at(iter)->display(where, scale, angle, renderer);

        ++iter;

        if (iter >= sprites.size())
        {
            iter = 0;
        }
    }

    virtual void save(Saver* os) const override
    {
        basic_sprite::save(os);

        os->save_data(sprites.size());
        for (sprite* x : sprites)
        {
            x->save(os);
        }

        os->save_data(iter);
    }

    virtual void load(Loader* os) override
    {
        basic_sprite::load(os);

        size_t list_size = os->load_data<size_t>();
        sprites.reserve(list_size);
        for (size_t x = 0; x < list_size; ++x)
        {
            sprite* v = os->load_complex_ptr<sprite>();
            sprites.push_back(v);
        }

        iter = os->load_data<size_t>();
    }
};

struct consistent_animation_sheet : public basic_sprite
{
ASSIGN_VIR_VAR_CONSTRUCTOR(consistent_animation_sheet);

public:
    SDL_Texture* texture = nullptr;

    size_t tile_x, tile_y; // Size of tiles

    size_t x = 0, y = 0; // Position of current tile on tilesheeet in tiles

    bool left_to_right = true; // Reads a left to right row, then the next column. Alternatively top to bottom

    consistent_animation_sheet()
    {
        // TODO
    }

    ~consistent_animation_sheet()
    {
        SDL_DestroyTexture(texture);
    }

    virtual SDL_Texture* get() override
    {
        return texture;
    }

    virtual void display(const pos& where, const pos& scale, double angle, SDL_Renderer* renderer) override
    {
        const SDL_FRect frect = pos::Make_SDL_FRect(where, half_size * scale);
        const SDL_FRect srect = {(float)tile_x * x, (float)tile_y * y, (float)((tile_x + size.x) * x), (float)((tile_y + size.y) * y)};
        SDL_RenderTextureRotated(renderer, texture, &srect, &frect, angle, nullptr, flip_mode);

        if (left_to_right)
        {
            ++x;
            if (tile_x * x >= size.x)
            {
                x = 0;
                ++y;

                if (tile_y * y >= size.y)
                {
                    y = 0;
                }
            }
        }

        else
        {
            ++y;
            if (tile_y * y >= size.y)
            {
                y = 0;
                ++x;

                if (tile_x * x >= size.x)
                {
                    x = 0;
                }
            }
        }
    }

    virtual void save(Saver* os) const override
    {
        basic_sprite::save(os);

        os->save_data(tile_x);
        os->save_data(tile_y);
        os->save_data(x);
        os->save_data(y);

        os->save_data(left_to_right);
    }

    virtual void load(Loader* os) override
    {
        basic_sprite::load(os);
        
        tile_x = os->load_data<size_t>();
        tile_y = os->load_data<size_t>();
        x = os->load_data<size_t>();
        y = os->load_data<size_t>();

        left_to_right = os->load_data<bool>();
    }
};