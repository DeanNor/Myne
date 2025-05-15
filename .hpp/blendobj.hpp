
#pragma once

#include "blend.h"

#include "object.hpp"

class BlendObj : public Object
{
REGISTER_OBJECT(BlendObj)

private:
    friend class cereal::access;

    template <class Archive>
    void save(Archive& ar) const
    {
        ar(cereal::base_class<Object>(this), size, image_size);
    }

    template <class Archive>
    void load(Archive& ar)
    {
        ar(cereal::base_class<Object>(this), size, image_size);
    }

protected:
    BLImage image;

    SDL_Texture* texture = nullptr;

    display* window = nullptr;

    pos image_size;
    pos size;

public:
    BlendObj();

    ~BlendObj();

    virtual void draw_overlay(pos origin);

    void update_image();

    void set_image(BLImage new_image);

    BLImage get_image();

    SDL_Texture* get_texture();

    void set_size(pos new_size);

    pos get_size();
};