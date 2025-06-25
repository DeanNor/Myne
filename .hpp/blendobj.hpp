
#pragma once

#include "factory.hpp"
#include "object.hpp"

#include "blend.h"

class BlendObj : public Object
{
ASSIGN_CONSTRUCTOR(BlendObj)

protected:
    BLImage image;

    SDL_Texture* texture = nullptr;

    display* window = nullptr;

    pos image_size;
    pos size;

public:
    BlendObj();

    ~BlendObj();

    virtual void save(Saver* ar) const
    {
        Object::save(ar);
        
        ar->save_complex(size);
        ar->save_complex(image_size);
    }

    virtual void load(Loader* ar)
    {
        Object::load(ar);
        
        size = ar->load_complex<pos>();
        image_size = ar->load_complex<pos>();
    }

    virtual void draw_overlay(pos origin);

    void update_image();

    void set_image(BLImage new_image);

    BLImage get_image();

    SDL_Texture* get_texture();

    void set_size(pos new_size);

    pos get_size();
};