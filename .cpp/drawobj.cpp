
#include "drawobj.h"

DrawObj::DrawObj()
{
    screen = get_current_game()->game_window;

    get_current_game()->draws.push_back(this);
}

void DrawObj::draw()
{
    if (sprite != nullptr)
    {
        pos computed = global_position.compute();
        double global_angle = global_position.compute_angle();
        al_draw_scaled_rotated_bitmap(sprite, center.x, center.y, computed.x, computed.y, scale.x, scale.y, global_angle, 0);
    }
}

void DrawObj::set_sprite(ALLEGRO_BITMAP* bitmap)
{
    sprite = bitmap;

    center.x = al_get_bitmap_width(sprite) / 2.0;
    center.y = al_get_bitmap_height(sprite) / 2.0;
}

ALLEGRO_BITMAP* DrawObj::get_sprite()
{
    return sprite;
}

void DrawObj::set_z(int new_z)
{
    z = new_z;
}

int DrawObj::get_z()
{
    return z;
}

void DrawObj::set_window(window* new_screen)
{
    screen = new_screen;
}

window* DrawObj::get_window()
{
    return screen;
}