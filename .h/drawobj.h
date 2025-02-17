
#pragma once

#include "object.h"

#include "window.h"

class DrawObj : public Object
{
protected:
    window* screen = nullptr;

    ALLEGRO_BITMAP* sprite = nullptr;
    pos center;

    pos scale = {1,1};

    int z = 0; // Draw depth

public:
    DrawObj();

    virtual void draw();

    void set_sprite(ALLEGRO_BITMAP* bitmap);

    ALLEGRO_BITMAP* get_sprite();

    void set_z(int new_z);

    int get_z();

    void set_window(window* new_screen);

    window* get_window();
};