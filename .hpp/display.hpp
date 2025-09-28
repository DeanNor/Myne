
#pragma once

#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"

#include ".hpp/pos.hpp"

struct display
{
private:
    SDL_Window* window;

    SDL_Renderer* renderer;

    pos center;
    pos size;
    pos half_size;

    pos top_left, top_right, bottom_left, bottom_right;

    pos real_size;
    pos screen_scale = {1,1};

    SDL_Texture* screen = nullptr;

    void update_corners()
    {
        top_left = center - half_size;
        top_right = {center.x + half_size.x, center.y - half_size.y};
        bottom_left = {center.x - half_size.x, center.y + half_size.y};
        bottom_right = center + half_size;
    }

    void update_real_size()
    {
        int size_x, size_y;
        SDL_GetWindowSizeInPixels(window, &size_x, &size_y);
        real_size = {(double)(size_x), (double)(size_y)};
    }

    void update_screen_size()
    {
        size = {(double)screen->w, (double)screen->h};

        half_size = size / 2.0;

        update_real_size();

        update_corners();
    }

public:
    display(pos display_size, const char* name, SDL_WindowFlags flags);

    ~display();

    void prepare_screen();

    void push_screen();

    void update_size()
    {
        update_real_size();

        if (!screen)
        {
            size = real_size;
            screen_scale = {1,1};

            half_size = size / 2.0;

            update_corners();
        }

        else screen_scale = real_size / size;
    }

    // New_screen is owned by this
    // New_screen should also have SDL_TextureAccess of SDL_TEXTUREACCESS_TARGET
    // NOTE: the scale mode for a texture should be applied to this as well
    void set_screen(SDL_Texture* new_screen)
    {
        SDL_DestroyTexture(screen);

        screen = new_screen;

        update_screen_size();
    }

    SDL_Texture* get_screen()
    {
        return screen;
    }

    pos get_size() const
    {
        return size;
    }

    pos get_half_size() const
    {
        return half_size;
    }

    pos get_top_left() const
    {
        return top_left;
    }

    pos get_top_right() const
    {
        return top_right;
    }

    pos get_bottom_left() const
    {
        return bottom_left;
    }

    pos get_bottom_right() const
    {
        return bottom_right;
    }

    void set_center(pos new_center)
    {
        center = new_center;

        update_corners();
    }

    pos get_center() const
    {
        return center;
    }

    pos get_scale() const
    {
        return screen_scale;
    }

    pos get_real_size() const
    {
        return real_size;
    }

    SDL_Window* get_window() const
    {
        return window;
    }

    SDL_Renderer* get_renderer() const
    {
        return renderer;
    }
};