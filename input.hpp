
#include ".hpp/pos.hpp"

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_keyboard.h"
#include "SDL3/SDL_keycode.h"
#include "SDL3/SDL_mouse.h"
#include <unordered_map>

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

struct keyboard_watcher
{
public:
    bool* value;
    SDL_Keycode watched;
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
                *(x->value) = true;
            }
        }
    }
};