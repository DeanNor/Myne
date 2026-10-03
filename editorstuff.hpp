#pragma once

#include ".hpp/SDL3.h"
#include ".hpp/hull.hpp"
#include ".hpp/process.hpp"
#include "SDL3/SDL_render.h"
#include "click.hpp"
#include "imgui-docking/imgui.h"
#include "imgui-docking/backends/imgui_impl_sdl3.h"
#include "imgui-docking/backends/imgui_impl_sdlrenderer3.h"
#include <cstddef>
#include <vector>

static const char* objects = "Scene Objects";
static const char* data = "Variables";
static const char* create = "Add Object";
static const char* viewport = "Editor";

#include ".hpp/game.hpp"

class EditorObj;
class DragObj;
class EditorManager;

struct editor : public game
{
private:
    bool mse_was_on_global = false;

    ClickManager click_manager;

    keyboard_watcher zoom_speed_watcher{SDLK_LCTRL};
    keyboard_watcher delete_watcher{SDLK_BACKSPACE};

    DragObj* dragged = nullptr;
    pos last_click;

    bool move_dragged = false;
    pos move_drag_start;
    pos move_drag_center;

    const constexpr static double rescale_scale = 1.1;
    const constexpr static double rescale_fast_scale = 1.25;

    EditorObj* current_selection = nullptr;
    std::size_t selection_loc = 0;
    EditorObj* editor_root = nullptr;

    EditorObj* queued_texture = nullptr;
    std::filesystem::path queued_str;

    struct
    {
        unsigned char r, g, b;
    }
        center_plus_x = {0xFF, 0x00, 0x00},
        center_plus_y = {0x00, 0x00, 0xFF},
        line_cube =     {0xFF, 0xFF, 0xFF};

    pos cube_size = {100,100};

    double scale_too_small = 0.3;

    enum 
    {
        NORMAL =    0,
        HULL =      1,
    } mode = NORMAL;

    void* mode_data;
    bool* mode_active = nullptr;

    std::vector<Process*> mode_nodes;

    ClickManager mode_clicks;

    void normal_mode();

    void hull_mode();

    void update_hull();

public:
    editor(const char* name, SDL_WindowFlags flags, pos window_size) : game(name, flags, window_size)
    {
        set_current_game(this);

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking

        ImGui::StyleColorsDark();

        ImGui_ImplSDL3_InitForSDLRenderer(game_window->get_window(), game_window->get_renderer());
        ImGui_ImplSDLRenderer3_Init(game_window->get_renderer());

        SDL_SetRenderDrawBlendMode(game_window->get_renderer(), SDL_BLENDMODE_BLEND);

        keyboard.add_watcher(&zoom_speed_watcher);
        keyboard.add_watcher(&delete_watcher);

        mode_data = new hull;
        mode = HULL;
    }
    
    ~editor();

    bool frame() override;

    // Modes
    void null_active_mode()
    {
        if (mode_active)
        {
            *mode_active = false;
        }

        for (Process* x : mode_nodes)
        {
            x->start_delete();
        }

        mode_nodes.clear();

        dragged = nullptr;

        mode_active = nullptr;
    }

    void enter_normal_mode()
    {
        null_active_mode();

        mode = NORMAL;
    }

    void enter_hull_save_mode(void* data, bool* data_active)
    {
        null_active_mode();

        mode = HULL;
        mode_data = data;
        *data_active = true;
        mode_active = data_active;
    }

    std::vector<Process*>& get_mode_nodes()
    {
        return mode_nodes;
    }

    void show_loadable_processes();

    void set_current_selection(EditorObj* new_selection)
    {
        current_selection = new_selection;
        enter_normal_mode();
    }

    EditorObj* get_current_selection() const
    {
        return current_selection;
    }

    void set_selection_location(std::size_t loc)
    {
        selection_loc = loc;
    }

    std::size_t get_selection_location()
    {
        return selection_loc;
    }

    void set_editor_root(EditorObj* new_editor_root)
    {
        editor_root = new_editor_root;
    }

    EditorObj* get_editor_root() const
    {
        return editor_root;
    }

    void set_loaded_img(void* drawer, std::string str)
    {
        queued_texture = (EditorObj*)drawer;
        queued_str = str;
    }

    ClickManager* get_click_manager()
    {
        return &click_manager;
    }

    void set_dragged(DragObj* new_dragged)
    {
        dragged = new_dragged;
    }

    DragObj* get_dragged()
    {
        return dragged;
    }
};

inline editor* editing_thing = nullptr;

inline void set_editor(editor* thing_that_edits)
{
    editing_thing = thing_that_edits;
}

inline editor* get_editor()
{
    return editing_thing;
}