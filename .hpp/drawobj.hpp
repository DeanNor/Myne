
#pragma once

#include ".hpp/object.hpp"

#include ".hpp/display.hpp"
#include <filesystem>

class DrawObj : public Object
{
ASSIGN_CONSTRUCTOR(DrawObj);

protected:
    display* window = nullptr;
    SDL_Texture* target = nullptr;

    SDL_Texture* sprite = nullptr;

    pos scale = {1,1};

    // Both size and half size
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

    // If any pixel of the object is on the screen. Calls compute.
    bool visible();

    // If 100% of the object is on the screen. Calls compute. Note, rounding errors (and moveover, physics) make this a little inaccurate.
    bool fully_visible();
};