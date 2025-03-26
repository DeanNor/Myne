
#pragma once

#include "object.hpp"

#include "display.hpp"

const rad RIGHT(0);
const rad DOWN(-1.5707963267948965579990);

class DrawObj : public Object
{
protected:
    display* window = nullptr;

    SDL_Texture* sprite = nullptr;
    pos center;

    pos scale = {1,1};

    int z = 0; // Draw depth

    bool ownership = false;

public:
    DrawObj();

    ~DrawObj();

    virtual void draw();

    void set_sprite(SDL_Texture* bitmap, bool owns_sprite);

    SDL_Texture* get_sprite();

    void set_z(int new_z);

    int get_z();

    void set_display(display* new_display);

    display* get_display();
};