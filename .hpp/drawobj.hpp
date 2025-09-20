
#pragma once

#include ".hpp/object.hpp"

#include ".hpp/display.hpp"
#include "SDL3/SDL_render.h"
#include <filesystem>

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

    bool active_drawer = true;

public:
    DrawObj();

    ~DrawObj();

    void load(Loader* ar) override;

    void save(Saver* ar) const override;

    virtual void draw(const pos& origin);

    void set_sprite(SDL_Texture* bitmap, bool owns_sprite);
    void set_sprite(std::filesystem::path path, SDL_ScaleMode scale_mode);

    SDL_Texture* get_sprite() const;

    void set_sprite_path(std::string path);

    std::string get_sprite_path() const;

    void set_depth(unsigned char depth);

    unsigned char get_depth() const;

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

    void make_active(bool is)
    {
        active_drawer = is;
    }

    bool is_active() const
    {
        return active_drawer;
    }
};