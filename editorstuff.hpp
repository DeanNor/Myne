#pragma once

#include ".hpp/SDL3.h"
#include "SDL3/SDL_render.h"
#include "imgui.h"
#include "imgui-docking/backends/imgui_impl_sdl3.h"
#include "imgui-docking/backends/imgui_impl_sdlrenderer3.h"

static const char* objects = "Scene Objects";
static const char* data = "Variables";
static const char* create = "Add Object";
static const char* viewport = "Editor";

#include ".hpp/game.hpp"

static const int viewport_size = 1000;

class EditorObj;
class EditorManager;

struct editor : public game
{
private:
    bool mse_was_on_global = false;

    EditorObj* current_selection = nullptr;
    EditorObj* editor_root = nullptr;

    EditorManager* editor_manager;

    EditorObj* queued_texture = nullptr;
    std::string queued_str;

    SDL_Texture* line_cube;
    pos cube_size;

    SDL_Texture* center_plus_x;
    SDL_Texture* center_plus_y;

    double scale_too_small = 0.3;

public:
    editor(const char* name, SDL_WindowFlags flags, pos window_size) : game(name, flags, window_size)
    {
        set_current_game(this);

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking

        ImGui::StyleColorsDark();

        ImGui_ImplSDL3_InitForSDLRenderer(game_window->get_window(), game_window->get_renderer());
        ImGui_ImplSDLRenderer3_Init(game_window->get_renderer());

        line_cube = load_img(get_game_window()->get_renderer(), "img/track2.png");
        cube_size = {100,100};

        center_plus_x = load_img(get_game_window()->get_renderer(), "img/track3.png");
        center_plus_y = load_img(get_game_window()->get_renderer(), "img/track4.png");
    }
    
    ~editor();

    bool frame() override;

    void show_loadable_processes();

    void set_current_selection(EditorObj* new_selection)
    {
        current_selection = new_selection;
    }

    EditorObj* get_current_selection() const
    {
        return current_selection;
    }

    void set_editor_root(EditorObj* new_editor_root)
    {
        editor_root = new_editor_root;
    }

    EditorObj* get_editor_root() const
    {
        return editor_root;
    }

    void set_editor_manager(EditorManager* new_editor_manager)
    {
        editor_manager = new_editor_manager;
    }

    EditorManager* get_editor_manager() const
    {
        return editor_manager;
    }

    void set_loaded_img(void* drawer, std::string str)
    {
        queued_texture = (EditorObj*)drawer;
        queued_str = str;
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