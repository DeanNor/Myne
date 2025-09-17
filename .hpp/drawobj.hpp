
#pragma once

#include ".hpp/object.hpp"

#include ".hpp/display.hpp"
#include "SDL3/SDL_render.h"
#include <filesystem>
#include <iostream>

class DrawTarget;

class DrawObj : public Object
{
ASSIGN_CONSTRUCTOR(DrawObj);

protected:
    SDL_Renderer* renderer = nullptr;

    bool has_target = true;
    union
    {
        display* window = nullptr;
        DrawTarget* draw_target;
    };

    SDL_Texture* sprite = nullptr;

    // TODO pos scale = {1,1};

    // Both size and half size of the sprite
    pos size;
    pos half_size;

    // If the sprite pointer is owned.
    bool sprite_ownership = false;

    SDL_ScaleMode sprite_scale_mode;

    std::string sprite_path;

    unsigned char depth = 0;

public:
    DrawObj();

    ~DrawObj();

    void load(Loader* ar) override;

    void save(Saver* ar) const override;

    virtual void draw(const pos& origin);

    void draw_diva(const pos& origin);

    void set_sprite(SDL_Texture* bitmap, bool owns_sprite);
    void set_sprite(std::filesystem::path path, SDL_ScaleMode scale_mode);

    SDL_Texture* get_sprite();

    void set_sprite_path(std::string path);

    std::string get_sprite_path();

    void set_depth(unsigned char depth);

    unsigned char get_depth();

    // Assumes that the depth is already assigned.
    void init();

    void init(unsigned char depth);

    // Set to use a custom draw target
    void init(DrawTarget* target);

    // If any pixel of the object is on the screen. Calls compute & compute_angle()
    bool visible();

    // If 100% of the object is on the screen. Calls compute. Note, rounding errors (and moveover, physics) make this a little inaccurate.
    bool fully_visible();

    pos get_size() const
    {
        return size;
    }

    pos get_half_size() const
    {
        return half_size;
    }
};

class DrawTarget : public DrawObj
{
protected:
    std::vector<DrawObj*> drawers;

    pos origin = {0,0};

public:
    virtual void draw(const pos& global_origin) override
    {
        SDL_Texture* global_draw_target = SDL_GetRenderTarget(renderer);
        SDL_SetRenderTarget(renderer, sprite);
        SDL_RenderClear(renderer);

        for (DrawObj* drawer : drawers)
        {
            drawer->draw_diva(origin - half_size);
        }

        SDL_SetRenderTarget(renderer, global_draw_target);

        DrawObj::draw(global_origin);
    }

    void add_to_draws(DrawObj* who)
    {
        drawers.push_back(who);
    }

    void remove_from_draws(DrawObj* who)
    {
        auto index = std::find(drawers.begin(), drawers.end(), who);

        if (index == drawers.end()) return;

        drawers.erase(index);
    }

    void set_origin(pos new_origin)
    {
        origin = new_origin;
    }

    pos get_origin() const
    {
        return origin;
    }

    pos get_zero()
    {
        return origin - half_size;
    }

    pos get_max()
    {
        return origin + half_size;
    }
};