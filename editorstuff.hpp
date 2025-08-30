#pragma once

#include ".hpp/SDL3.h"
#include "SDL3/SDL_events.h"
#include "imgui-docking/imgui.h"
#include "imgui-docking/backends/imgui_impl_sdl3.h"
#include "imgui-docking/backends/imgui_impl_sdlrenderer3.h"
#include <filesystem>

static const char* objects = "Scene Objects";
static const char* data = "Variables";
static const char* create = "Add Object";
static const char* viewport = "Editor";

class Dragable;
class EditorObj;

#include ".hpp/game.hpp"

static const int viewport_size = 1000;

static const ImGuiTreeNodeFlags tree_flags = ImGuiTreeNodeFlags_DrawLinesFull | ImGuiTreeNodeFlags_DefaultOpen;

namespace EDITOR
{
    static SDL_Texture* basic_positional;

    inline void setup_namespace()
    {
        load_img(basic_positional, get_current_game()->game_window->renderer, "img/track1.png");
    }
}

ImGuiIO& init_imgui()
{
    ImGui::CreateContext();
    return ImGui::GetIO();
}

struct editor : public game
{
private:
    Dragable* dragged = nullptr;
    pos last_mouse;

    EditorObj* editor_root = nullptr;

    ImGuiIO& io;

public:
    editor(const char* name, SDL_WindowFlags flags) : game(name, flags), io(init_imgui())
    {
        set_current_game(this);

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking

        ImGui::StyleColorsDark();

        ImGui_ImplSDL3_InitForSDLRenderer(game_window->window, game_window->renderer);
        ImGui_ImplSDLRenderer3_Init(game_window->renderer);
    }

    virtual bool frame();

    ~editor()
    {
        ImGui_ImplSDLRenderer3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
    }

    void check_clicked();

    void add_to_clicks(Dragable* what);

    void set_editor_root(EditorObj* new_editor_root);
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

#include "editortests.hpp"

class EditorObj : public Dragable
{
ASSIGN_CONSTRUCTOR(EditorObj);

public:
    std::string represents;

    bool child = false;

    bool has_sprite = false;

    void load(Loader* load) override
    {
        Dragable::load(load);
        
        if (!has_sprite)
        {
            set_sprite(EDITOR::basic_positional, false);
        }

        pos wh(sprite->w,sprite->h);
        set_size(wh / 2);
    }

    void _process() override
    {
        process();

        if (ImGui::TreeNodeEx(this, tree_flags, "%s", represents.c_str()))
        {
            process_children(); // Do all child objects, then finish the tree

            ImGui::TreePop();
        }
    }
};

void editor::check_clicked()
{
    dragged = editor_root->check_click(mouse.position);
}

void editor::set_editor_root(EditorObj* new_editor_root)
{
    editor_root = new_editor_root;
}

bool editor::frame()
{
    // basically view_events(); but with a ImGui process as well
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        ImGui_ImplSDL3_ProcessEvent(&event);
        process_event(event);

        if (!io.WantCaptureMouse && editor_root)
        {
            if (dragged == nullptr)
            {
                if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT)
                {
                    check_clicked();

                    last_mouse = mouse.position;
                }
            }

            else
            {
                if (event.type == SDL_EVENT_MOUSE_BUTTON_UP && event.button.button == SDL_BUTTON_LEFT)
                {
                    dragged = nullptr;
                }

                else
                {
                    dragged->set_position((dragged->get_position() + (mouse.position - last_mouse)).round());
                    last_mouse = mouse.position;
                }
            }
        }
    }
    
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    //ImGui::ShowDemoWindow();

    ImGui::Begin(objects, nullptr);
    run_processes();
    ImGui::End();

    ImGui::Begin(data, nullptr);
    ImGui::Text("Fps: %.4f", 1 / delta);
    // TODO a lot. take the categories of objects from below and find their datas
    ImGui::End();

    ImGui::Begin(create, nullptr);
    // TODO a lot. auto categorize objects that call ASSIGN_CONSTRUCTOR() and add them here
    ImGui::End();

    wait_for_frame();

    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), game_window->renderer);

    finish_processes();

    return running;
}