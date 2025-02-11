
#pragma once

#ifndef WINDOW_H
#define WINDOW_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>

#include <string>
#include <iostream>

static ALLEGRO_COLOR WHITE = al_map_rgb_f(1,1,1);
static ALLEGRO_COLOR BLACK = al_map_rgb_f(0,0,0);
static ALLEGRO_COLOR CLEAR = al_map_rgba_f(0,0,0,0);

struct window
{
    ALLEGRO_DISPLAY* display = nullptr;
    
    ALLEGRO_EVENT_QUEUE* queue = nullptr;

    ALLEGRO_BITMAP* background = nullptr;
    ALLEGRO_BITMAP* foreground = nullptr;

    window(int x, int y, ALLEGRO_EVENT_QUEUE* event_queue, std::string name);

    ~window();

    void push_screen();
};

#endif