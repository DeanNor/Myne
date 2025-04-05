
#pragma once

#include "blend.h"

#include "object.hpp"

class BlendObj : public Object
{
protected:
    BLImage image;

    SDL_Texture* texture = nullptr;

    display* window = nullptr;

    pos image_size;
    pos size;

public:
    BlendObj();

    ~BlendObj();

    virtual void draw_overlay();

    void update_image();

    void set_image(BLImage new_image);

    BLImage get_image();

    void set_size(pos new_size);

    pos get_size();
};