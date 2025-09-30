#pragma once

#include ".hpp/SDL3.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_surface.h"
#include "SDL3/SDL_timer.h"
#include "ast/ast_load.hpp"
#include "ast/print.hpp"
#include "imgui.h"
#include "imgui-docking/backends/imgui_impl_sdl3.h"
#include "imgui-docking/backends/imgui_impl_sdlrenderer3.h"

static const char* objects = "Scene Objects";
static const char* data = "Variables";
static const char* create = "Add Object";
static const char* viewport = "Editor";

class Dragable;
class EditorObj;

#include ".hpp/game.hpp"

static const int viewport_size = 1000;

namespace EDITOR
{
    static SDL_Texture* basic_positional = nullptr;

    inline void setup_namespace()
    {
        load_img(basic_positional, get_current_game()->get_game_window()->get_renderer(), "img/track1.png", SDL_SCALEMODE_PIXELART);
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

    EditorObj* current_selection = nullptr;

public:
    editor(const char* name, SDL_WindowFlags flags, pos window_size) : game(name, flags, window_size), io(init_imgui())
    {
        set_current_game(this);

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking

        ImGui::StyleColorsDark();

        ImGui_ImplSDL3_InitForSDLRenderer(game_window->get_window(), game_window->get_renderer());
        ImGui_ImplSDLRenderer3_Init(game_window->get_renderer());
    }

    virtual bool frame();

    ~editor()
    {
        ImGui_ImplSDLRenderer3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
    }

    void list_values();

    void check_clicked();

    void add_to_clicks(Dragable* what);

    void set_editor_root(EditorObj* new_editor_root);

    void set_current_selection(EditorObj* selection);

    EditorObj* get_current_selection();
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

#include "ast/ast_custom.hpp"

class EditorObj : public Dragable
{
ASSIGN_CONSTRUCTOR(EditorObj);

public:
    ast_expands_copy* expansion = nullptr;

    ast_process* process_class_ast = nullptr;
    ast_object* object_class_ast = nullptr;
    ast_drawobj* drawobj_class_ast = nullptr;

    EditorObj() = default;

    EditorObj(hash_t type_id)
    {
        ast_expands* type = loadable_processes[type_id];
        if (type)
        {
            expansion = dynamic_cast<ast_expands_copy*>(type->copy());

            if (expansion)
            {
                process_class_ast = expansion->get_expansion_part<ast_process>();
                if (process_class_ast)
                {
                    name.resize(MAX_STRING_LENGTH);
                    process_class_ast->obj_name = &name;
                }

                object_class_ast = expansion->get_expansion_part<ast_object>();
                if (object_class_ast) object_class_ast->position = &position;

                drawobj_class_ast = expansion->get_expansion_part<ast_drawobj>();
                if (drawobj_class_ast) drawobj_class_ast->texture = sprite;
            }
        }
    }
    
    void load(Loader* load) override
    {
        Dragable::load(load);
        
        if (!sprite)
        {
            set_sprite(EDITOR::basic_positional, false);
        }

        pos wh(sprite->w,sprite->h);
        set_size(wh / 2);
    }

private:
    inline void run_child_in_imgui_tree()
    {
        process_children(); // Do all child objects, then finish the tree

        ImGui::TreePop();
    }

public:
    void _process() override
    {
        process();

        if (process_class_ast)
        {
            if (ImGui::TreeNodeEx(this, tree_flags, "%s", name.data()))
            {
                run_child_in_imgui_tree();
            }
        }

        else
        {
            if (ImGui::TreeNodeEx(this, tree_flags, "ERROR"))
            {
                run_child_in_imgui_tree();
            }
        }
    }

    void display_expansion()
    {
        if (expansion != nullptr)
        {
            expansion->use_editor();
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
    Uint64 tick_count = SDL_GetTicks();
    Uint64 ticks = tick_count - total_ticks;
    Uint64 coll_ticks = tick_count - total_coll_ticks;
    Uint64 frame_ticks = tick_count - total_frame_ticks;

    int count = floor(ticks / fpsticks);
    int coll_count = physics ? floor(coll_ticks / collticks) : 0;
    bool frame_count = frame_ticks >= frameticks;

    if ((count > 0 || coll_count > 0) && frame_count)
    {
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        //update_run_data();
        // Reset just_pressed-s
        keyboard.reset();
        mouse.reset();

        //view_events();
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);
            process_event(event);

            switch (event.type)
            {
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                if (event.window.windowID == SDL_GetWindowID(game_window->get_window()))
                {
                    running = false;
                }
                break;
                
            case SDL_EVENT_QUIT:
                running = false;
                break;

            case SDL_EVENT_WINDOW_RESIZED:
                game_window->update_size();
                break;

            case SDL_EVENT_MOUSE_BUTTON_DOWN:
            case SDL_EVENT_MOUSE_BUTTON_UP:
                mouse.recheck(event.button);
                break;

            case SDL_EVENT_KEY_DOWN:
            case SDL_EVENT_KEY_UP:
                keyboard.recheck(event.key);
            }
        }

        // Update mouse position
        float x,y;
        SDL_GetMouseState(&x,&y);
        mouse.position = (pos(x,y) / game_window->get_scale()) + game_window->get_center() - game_window->get_half_size();
        
        do
        {
            if (count > 0)
            {
                total_ticks = tick_count;


                ImGui::Begin(objects, nullptr);
                run_processes();
                ImGui::End();

                ImGui::Begin(data, nullptr);
                list_values();
                ImGui::End();

                ImGui::Begin(create, nullptr);
                ImGui::End();

                --count;
            }

            if (physics)
            {
                if (coll_count > 0)
                {
                    total_coll_ticks = tick_count;

                    run_collision();
                    --coll_count;
                }
            }

        } while (count > 0 || coll_count > 0);

        run_frame();

        ImGui::Render();
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), game_window->get_renderer());

        total_frame_ticks = tick_count;

        end_delete();
    }

    else
    {
        SDL_Delay(std::min({fpsticks - ticks, collticks - coll_ticks, frameticks - frame_ticks}));
    }

    return running;
}

void editor::list_values()
{
    if (current_selection)
    {
        current_selection->display_expansion();
    }
}

void editor::set_current_selection(EditorObj* selection)
{
    current_selection = selection;
}

EditorObj* editor::get_current_selection()
{
    return current_selection;
}