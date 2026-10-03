
#pragma once

#include "object.hpp"

#include "display.hpp"
#include "sprite.hpp"

#include <filesystem>

class DrawTarget;

#ifdef EDITOR
class ast_drawobj;
#endif

// Needs to call set_depth or target in order to draw
class DrawObj : public Object
{
ASSIGN_CONSTRUCTOR(DrawObj);

#ifdef EDITOR
friend ast_drawobj;
#endif

protected:
    SDL_Renderer* renderer = nullptr;

    bool sprite_ownership = false;

    unsigned char depth;

    pos scale = {1,1};

    bool has_target = false;
    union
    {
        display* window = nullptr;
        DrawTarget* draw_target;
    };

    basic_sprite* texture = nullptr;

    bool active_drawer = true;

    bool initialized = false;

public:
    DrawObj();

    ~DrawObj();

    void load(Loader* ar) override;

    void save(Saver* ar) const override;

    virtual void draw(const pos& origin, const pos& global_scale);

    void set_sprite(basic_sprite* new_sprite, bool owns_sprite);
    void set_sprite(std::filesystem::path path, SDL_ScaleMode scale_mode = SDL_SCALEMODE_PIXELART);

    basic_sprite* get_texture() const;

    void set_depth(unsigned char depth);

    unsigned char get_depth() const;

    // Set to use a custom draw target
    void target(DrawTarget* target);

    // If any pixel of the object is on the screen. Calls compute & compute_angle()
    // Will only return true for sprite versions of the texture
    bool visible();

    // If 100% of the object is on the screen. Calls compute. Note, rounding errors (and moveover, physics) make this a little inaccurate.
    // Will only return true for sprite versions of the texture // TODO add a virtual function to do this in basic_sprite
    bool fully_visible();

    // Can draw = is
    void set_active(bool is)
    {
        active_drawer = is;
    }

    bool is_active() const
    {
        return active_drawer;
    }

    void set_scale(pos new_scale)
    {
        scale = new_scale;
    }

    pos get_scale() const
    {
        return scale;
    }
};