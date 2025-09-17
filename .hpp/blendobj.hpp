
#pragma once

#include "SDL3/SDL_render.h"

#include ".hpp/blend.h"

#include ".hpp/display.hpp"

#include ".hpp/object.hpp"

// Requires that init is called and the size is set for drawing
class BlendObj : public Object
{
ASSIGN_CONSTRUCTOR(BlendObj);

protected:
    BLImage image;

    SDL_Texture* texture = nullptr;

    display* window = nullptr;

    pos image_size;
    pos size = {0,0};

    unsigned char depth = 0;

public:
    BlendObj();

    ~BlendObj();

    virtual void save(Saver* ar) const override
    {
        Object::save(ar);
        
        ar->save_complex(size);
        ar->save_complex(image_size);
    }

    virtual void load(Loader* ar) override
    {
        Object::load(ar);
        
        size = ar->load_complex<pos>();
        image_size = ar->load_complex<pos>();
    }

    virtual void draw_overlay(pos origin);

    void update_image();

    void init();

    void init(unsigned char z);

    // Updates image (calling update_image) to new_image and sets it
    void set_image(BLImage new_image);

    BLImage get_image();

    SDL_Texture* get_texture();

    void set_size(pos new_size);

    pos get_size();

    void set_depth(unsigned char depth);

    unsigned char get_depth();
};