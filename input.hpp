
#include ".hpp/factory.hpp"
#include ".hpp/loader.hpp"
#include ".hpp/pos.hpp"

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_keycode.h"
#include "SDL3/SDL_mouse.h"
#include "SDL3/SDL_stdinc.h"

struct mouse_state
{
public:
    pos position;

    bool ldown = false;
    bool mdown = false;
    bool rdown = false;
    bool x1down = false;
    bool x2down = false;

    mouse_state() = default;

    mouse_state(const mouse_state&) = delete;

    void recheck(SDL_MouseButtonEvent& mse)
    {
        switch(mse.button)
        {
        case SDL_BUTTON_LEFT:
            ldown = mse.down;

        case SDL_BUTTON_MIDDLE:
            mdown = mse.down;
            
        case SDL_BUTTON_RIGHT:
            rdown = mse.down;

        case SDL_BUTTON_X1:
            x1down = mse.down;

        case SDL_BUTTON_X2:
            x2down = mse.down;
        }
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