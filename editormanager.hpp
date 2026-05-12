
#pragma once

#include ".hpp/game.hpp"
#include "SDL3/SDL_keycode.h"
#include "editorobj.hpp"
#include "click.hpp"
#include "editorstuff.hpp"

class EditorManager : public Process
{
friend EditorObj;

private:
    mouse_state& mse;
    display* window;

    ClickManager click_manager;

    keyboard_watcher zoom_speed_watcher{SDLK_LCTRL};
    keyboard_watcher delete_watcher{SDLK_BACKSPACE};

    EditorObj* dragged = nullptr;
    pos last_click;

    bool move_dragged = false;
    pos move_drag_start;
    pos move_drag_center;

    const constexpr static double rescale_scale = 1.1;
    const constexpr static double rescale_fast_scale = 1.25;

public:
    EditorManager() : mse(get_current_game()->get_mouse()), window(get_current_game()->get_game_window())
    {
        get_current_game()->get_keyboard().add_watcher(&zoom_speed_watcher);
        get_current_game()->get_keyboard().add_watcher(&delete_watcher);
    }

    ~EditorManager()
    {
        get_current_game()->get_keyboard().remove_watcher(&zoom_speed_watcher);
        get_current_game()->get_keyboard().remove_watcher(&delete_watcher);
    }

    virtual void process() override
    {
        if (dragged)
        {
            if (mse.ljust_released)
            {
                dragged = nullptr;
            }

            else dragged->get_tfm()->set(mse.position.round());
        }

        if (mse.mmoved_y)
        {
            pos past_size = window->get_size();

            pos past_scale = window->get_scale();
            pos changed_scale;
            
            if (mse.mspeed.y > 0)
            {
                changed_scale = past_scale * (zoom_speed_watcher.down() ? rescale_fast_scale : rescale_scale);
            }

            else
            {
                changed_scale = past_scale / (zoom_speed_watcher.down() ? rescale_fast_scale : rescale_scale);
            }

            window->set_scale(changed_scale);

            pos mse_center = mse.global_position - window->get_real_half_size();
            window->set_center(window->get_center() + ((mse_center / past_scale) - (mse_center / changed_scale)));
        }

        if (mse.ljust_down)
        {
            click_manager.handle_click(mse.position, this, window->get_scale());
        }

        if (mse.rjust_down)
        {
            move_drag_start = mse.global_position;
            move_drag_center = window->get_center();
            move_dragged = true;
        }

        if (move_dragged)
        {
            window->set_center(move_drag_center - (mse.global_position - move_drag_start) / window->get_scale());

            if (mse.rjust_released)
            {
                move_dragged = false;
            }
        }

        if (delete_watcher.just_pressed())
        {
            EditorObj* current_selection = get_editor()->get_current_selection();

            if (current_selection)
            {
                current_selection->start_delete();

                if (get_editor()->get_editor_root() == current_selection)
                {
                    get_editor()->set_editor_root(nullptr);
                }

                get_editor()->set_current_selection(nullptr);
            }
        }
    }
};
