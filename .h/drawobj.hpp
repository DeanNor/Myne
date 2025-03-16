
#pragma once

#include "object.hpp"

#include "display.hpp"

class DrawObj : public Object
{
protected:
    display* window = nullptr;

    SDL_Texture* sprite = nullptr;
    pos center;

    pos scale = {1,1};

    int z = 0; // Draw depth

public:
    DrawObj();

    virtual void draw();

    void set_sprite(SDL_Texture* bitmap);

    SDL_Texture* get_sprite();

    void set_z(int new_z);

    int get_z();

    void set_display(display* new_display);

    display* get_display();
};