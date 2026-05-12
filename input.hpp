
#include ".hpp/factory.hpp"
#include ".hpp/loader.hpp"
#include ".hpp/pos.hpp"

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_keycode.h"
#include "SDL3/SDL_mouse.h"
#include "SDL3/SDL_stdinc.h"
#include <stdexcept>

struct mouse_state
{
public:
    // Position relative to the game zero
    pos position;

    // Unscaled Position relative to top left screen. A little bit of a lie because it does not go outside of the screen
    pos global_position;

    bool ldown = false;
    bool mdown = false;
    bool rdown = false;
    bool x1down = false;
    bool x2down = false;

    bool ljust_down = false;
    bool mjust_down = false;
    bool rjust_down = false;
    bool x1just_down = false;
    bool x2just_down = false;

    bool ljust_released = false;
    bool mjust_released = false;
    bool rjust_released = false;
    bool x1just_released = false;
    bool x2just_released = false;

    bool mmoved_x = false;
    bool mmoved_y = false;

    bool mjust_moved_x = false;
    bool mjust_moved_y = false;

    bool mjust_stopped_x = false;
    bool mjust_stopped_y = false;

    // Middle scroll speed
    pos mspeed = {0,0};

    // Global speed
    pos speed = {0,0};

    mouse_state() = default;

    mouse_state(const mouse_state&) = delete;

    void move(SDL_MouseMotionEvent& mse)
    {
        speed = {mse.x - global_position.x, mse.y - global_position.y};
    }

    void scroll(SDL_MouseWheelEvent& mse) // TODO decide if we use integer_x as well, for mouse 'ticks'
    {
        if (mse.direction == SDL_MOUSEWHEEL_FLIPPED)
        {
            mspeed.x = -mse.x;
            mspeed.y = -mse.y;
        }

        else
        {
            mspeed.x = mse.x;
            mspeed.y = mse.y;
        }

        mjust_moved_x = mse.x && !mmoved_x;
        mjust_moved_y = mse.y && !mmoved_y;

        mjust_stopped_x = !mse.x && mmoved_x;
        mjust_stopped_y = !mse.y && mmoved_y;

        mmoved_x = mse.x != 0;
        mmoved_y = mse.y != 0;
    }

    void recheck(SDL_MouseButtonEvent& mse)
    {
        switch(mse.button)
        {
        case SDL_BUTTON_LEFT:
            ljust_down = mse.down && !ldown;
            ljust_released = !mse.down && ldown;
            ldown = mse.down;
            break;

        case SDL_BUTTON_MIDDLE:
            mjust_down = mse.down && !mdown;
            mjust_released = !mse.down && mdown;
            mdown = mse.down;
            break;
            
        case SDL_BUTTON_RIGHT:
            rjust_down = mse.down && !rdown;
            rjust_released = !mse.down && rdown;
            rdown = mse.down;
            break;

        case SDL_BUTTON_X1:
            x1just_down = mse.down && !x1down;
            x1just_released = !mse.down && x1down;
            x1down = mse.down;
            break;

        case SDL_BUTTON_X2:
            x2just_down = mse.down && !x2down;
            x2just_released = !mse.down && x2down;
            x2down = mse.down;
            break;
        }
    }

    void reset()
    {
        ljust_down = false;
        rjust_down = false;
        mjust_down = false;
        x1just_down = false;
        x2just_down = false;

        ljust_released = false;
        mjust_released = false;
        rjust_released = false;
        x1just_released = false;
        x2just_released = false;

        mmoved_x = false;
        mmoved_y = false;

        mjust_moved_x = false;
        mjust_moved_y = false;

        mjust_stopped_x = false;
        mjust_stopped_y = false;

        mspeed = {0,0};

        speed = {0,0};
    }

    // Reset all, including clicked
    void blank()
    {
        ljust_down = false;
        rjust_down = false;
        mjust_down = false;
        x1just_down = false;
        x2just_down = false;

        ljust_released = ldown;
        mjust_released = mdown;
        rjust_released = rdown;
        x1just_released = x1down;
        x2just_released = x2down;

        mjust_moved_x = false;
        mjust_moved_y = false;

        mjust_stopped_x = mmoved_x;
        mjust_stopped_y = mmoved_y;

        mspeed = {0,0};

        speed = {0,0};

        ldown = false;
        mdown = false;
        rdown = false;
        x1down = false;
        x2down = false;

        mmoved_x = false;
        mmoved_y = false;
    }
};

struct keyboard_state;

struct keyboard_watcher
{
ASSIGN_VAR_CONSTRUCTOR(keyboard_watcher);

friend keyboard_state;

private:
    bool pressed_now = false;
    bool released_now = false;
    bool pressed = false;

    uint64_t last_press_time = 0;

public:
    SDL_Keycode watched;

    keyboard_watcher() = default;

    keyboard_watcher(SDL_Keycode key) : watched(key)
    {

    }

    bool down() const
    {
        return pressed;
    }

    bool just_pressed() const
    {
        return pressed_now;
    }

    bool just_released() const
    {
        return released_now;
    }

    Uint64 get_press_time() const
    {
        return last_press_time;
    }

    void load(Loader* loader)
    {
        watched = loader->load_data<SDL_Keycode>();
    }

    void save(Saver* saver)
    {
        saver->save_data(watched);
    }
};

struct keyboard_state // TODO saveable and loadable
{
private:
    std::vector<keyboard_watcher*> watchers;

public:
    bool changed;
    SDL_Keycode recent_change;

    void add_watcher(keyboard_watcher* watcher)
    {
        watchers.push_back(watcher);
    }

    void remove_watcher(keyboard_watcher* watcher)
    {
        auto index = std::find(watchers.begin(), watchers.end(), watcher);

        if (index == watchers.end())
        {
            throw std::invalid_argument("No watcher found");
        }

        watchers.erase(index);
    }

    void recheck(SDL_KeyboardEvent key_change)
    {
        changed = true;
        recent_change = key_change.key;

        for (keyboard_watcher* x : watchers)
        {
            if (x->watched == recent_change)
            {
                if (key_change.down)
                {
                    x->pressed_now = !x->pressed;

                    if (x->pressed_now) x->last_press_time = key_change.timestamp;
                }

                else
                {
                    x->released_now = x->pressed;
                }

                x->pressed = key_change.down;
            }
        }
    }

    void reset()
    {
        for (keyboard_watcher* x : watchers)
        {
            x->pressed_now = false;
            x->released_now = false;
        }

        recent_change = SDLK_UNKNOWN;
    }
};