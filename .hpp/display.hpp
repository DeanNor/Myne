
#pragma once

#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"

#include ".hpp/pos.hpp"

struct display
{
private:
    SDL_Window* window;

    SDL_Renderer* renderer;

    pos center = {0,0};
    pos size;
    pos half_size;

    pos top_left, top_right, bottom_left, bottom_right;

    pos real_size;
    pos real_half_size;
    pos screen_scale = {1,1};

    pos scale = {1,1};

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
        real_half_size = real_size / 2.;
    }

    void compute_screen_size()
    {
        size = pos{(double)screen->w, (double)screen->h} / scale;

        half_size = real_half_size / scale;

        update_corners();
    }

    void compute_window_size()
    {
        update_real_size();

        size = real_size / scale;

        half_size = real_half_size / scale;

        update_corners();
    }

public:
    display(const pos& display_size, const char* name, SDL_WindowFlags flags);

    ~display();

    void prepare_screen();

    void push_screen();

    void update_size()
    {
        if (!screen)
        {
            compute_window_size();

            screen_scale = {1,1};
        }

        else
        {
            update_real_size();

            screen_scale = real_size / size;
        }
    }

    // New_screen is owned by this
    // New_screen should also have SDL_TextureAccess of SDL_TEXTUREACCESS_TARGET
    // NOTE: the scale mode for a texture should be applied to this as well
    void set_screen(SDL_Texture* new_screen)
    {
        SDL_DestroyTexture(screen);

        screen = new_screen;

        compute_screen_size();
    }

    SDL_Texture* get_screen()
    {
        return screen;
    }

    const pos& get_size() const
    {
        return size;
    }

    const pos& get_half_size() const
    {
        return half_size;
    }

    const pos& get_top_left() const
    {
        return top_left;
    }

    const pos& get_top_right() const
    {
        return top_right;
    }

    const pos& get_bottom_left() const
    {
        return bottom_left;
    }

    const pos& get_bottom_right() const
    {
        return bottom_right;
    }

    void set_center(pos new_center);

    const pos& get_center() const
    {
        return center;
    }

    void set_scale(pos new_scale);

    const pos& get_scale() const
    {
        return scale;
    }

    // Scale of the monitor to the backbuffer, not the rendering scale. You most likely want get_display_scale()
    const pos& get_screen_scale() const
    {
        return screen_scale;
    }

    const pos& get_real_size() const
    {
        return real_size;
    }

    const pos& get_real_half_size() const
    {
        return real_half_size;
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