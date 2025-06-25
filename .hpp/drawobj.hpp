
#pragma once

#include "factory.hpp"
#include "object.hpp"

#include "display.hpp"
#include "saver.hpp"

class DrawObj : public Object
{
ASSIGN_CONSTRUCTOR(DrawObj)

public:
    void load(Loader* ar)
    {
        Object::load(ar);

        center = ar->load_complex<pos>();
        scale = ar->load_complex<pos>();
        ownership = ar->load_data<bool>();

        if (ownership)
        {
            sprite_path = ar->load_complex<std::string>();

            set_sprite(sprite_path);
        }
    }

    void save(Saver* ar) const
    {
        Object::save(ar);
        
        ar->save_complex(center);
        ar->save_complex(scale);
        ar->save_data(ownership);

        if (ownership)
        {
            ar->save_complex(sprite_path);
        }
    }

protected:
    display* window = nullptr;
    SDL_Texture* target = nullptr;

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

    // If any pixel of the object is on the screen.
    bool visible();

    // If 100% of the object is on the screen.
    bool fully_visible();
};