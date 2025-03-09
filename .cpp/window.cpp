
#include "window.hpp"

window::window(pos window_size, ALLEGRO_EVENT_QUEUE* event_queue, std::string name)
{
    size = window_size;
    center = size / 2.0;
    half_size = size / 2;

    al_set_new_display_flags(ALLEGRO_OPENGL_3_0);

    display = al_create_display(size.x, size.y);

    queue = event_queue;

    al_register_event_source(queue, al_get_display_event_source(display));

    al_set_window_title(display, name.c_str());

    background = al_create_bitmap(size.x,size.y);
    foreground = al_create_bitmap(size.x,size.y);

    al_set_target_bitmap(background);
    al_clear_to_color(CLEAR);
}

window::~window()
{
    al_unregister_event_source(queue, al_get_display_event_source(display));
    al_destroy_display(display);
}

void window::push_screen()
{
    al_set_target_backbuffer(display);
    al_draw_bitmap(background, 0, 0, 0);
    //al_draw_bitmap(foreground, 0, 0, 0);

    al_flip_display();
    al_clear_to_color(CLEAR);

    al_set_target_bitmap(background);
    al_clear_to_color(CLEAR);

    //al_set_target_bitmap(foreground);
    //al_clear_to_color(CLEAR);
}