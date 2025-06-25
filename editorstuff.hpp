#pragma once

#include "imgui-docking/imgui.h"
#include "imgui-docking/backends/imgui_impl_sdl3.h"
#include "imgui-docking/backends/imgui_impl_sdlrenderer3.h"
#include "imgui-docking/misc/cpp/imgui_stdlib.h"

static const char* objects = "Scene Objects";
static const char* data = "Variables";
static const char* create = "Add Object";
static const char* viewport = "Editor";

#include "game.hpp"

static const int viewport_size = 1000;

static const ImGuiTreeNodeFlags tree_flags = ImGuiTreeNodeFlags_DrawLinesFull | ImGuiTreeNodeFlags_DefaultOpen;

namespace EDITOR
{
    static SDL_Texture* basic_positional;

    void setup_namespace()
    {
        load_img(basic_positional, get_current_game()->game_window->renderer, "img/track.png");
    }
}

struct editor : public game
{
public:
    editor(const char* name, SDL_WindowFlags flags) : game(name, flags)
    {
        set_current_game(this);

        ImGui::CreateContext(); // Could store a context, but not required right now

        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking

        ImGui::StyleColorsDark();

        ImGui_ImplSDL3_InitForSDLRenderer(game_window->window, game_window->renderer);
        ImGui_ImplSDLRenderer3_Init(game_window->renderer);
    }

    virtual bool frame()
    {
        // basically view_events(); but with a ImGui process as well
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);
            process_event(event);
        }
        
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        //ImGui::ShowDemoWindow();

        ImGui::Begin(objects, nullptr);
        run_processes();
        ImGui::End();

        ImGui::Begin(data, nullptr);
        ImGui::Text("Fps: %.4f", 1 / delay);
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

    ~editor()
    {
        ImGui_ImplSDLRenderer3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
    }
};

editor* editing_thing = nullptr;

void set_editor(editor* thing_that_edits)
{
    editing_thing = thing_that_edits;
}

editor* get_editor()
{
    return editing_thing;
}

#include "editortests.hpp"

class EditorObj : public Dragable
{
ASSIGN_CONSTRUCTOR(EditorObj)

ARCHIVE_INHERIT(Dragable)

public:
    const char* represents = "";

    bool child = false;

    bool has_sprite = false;

    void onload()
    {
        if (!has_sprite)
        {
            set_sprite(EDITOR::basic_positional, false);
        }

        pos wh(sprite->w,sprite->h);
        set_size(wh / 2);
    }

    void _process(double delta)
    {
        process(delta);

        if (ImGui::TreeNodeEx(this, tree_flags, "%s", represents))
        {
            process_children(delta); // Do all child objects, then finish the tree

            ImGui::TreePop();
        }
    }
};