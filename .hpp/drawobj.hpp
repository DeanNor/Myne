
#pragma once

#include "object.hpp"

#include "display.hpp"

class DrawObj : public Object
{
REGISTER_OBJECT(DrawObj)

public:
    template <class Archive>
    void load(Archive& ar)
    {
        ar(cereal::base_class<Object>(this));
        ar(center, scale, ownership);

        if (ownership)
        {
            ar(sprite_path);

            set_sprite(sprite_path);
        }
    }

    template <class Archive>
    void save(Archive& ar) const
    {
        ar(cereal::base_class<Object>(this));
        ar(center, scale, ownership);

        if (ownership)
        {
            ar(sprite_path);
        }
    }

protected:
    display* window = nullptr;

    SDL_Texture* sprite = nullptr;
    pos center;

    pos scale = {1,1};

    bool ownership = false;

    std::string sprite_path;

public:
    DrawObj();

    ~DrawObj();

    virtual void draw(pos origin);

    void set_sprite(SDL_Texture* bitmap, bool owns_sprite);
    void set_sprite(std::string path);

    SDL_Texture* get_sprite();

    void set_sprite_path(std::string path);

    std::string get_sprite_path();

    void set_display(display* new_display);

    display* get_display();

    bool visible();

    bool fully_visible();
};