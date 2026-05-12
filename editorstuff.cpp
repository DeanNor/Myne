
#include "editorstuff.hpp"

#include ".hpp/display.hpp"
#include ".hpp/game.hpp"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_timer.h"
#include "ast/ast_stuff.hpp"
#include "editormanager.hpp"
#include "imgui.h"
#include <SDL3/SDL_rect.h>

const char* draggable_id = "draggable";

editor::~editor()
{
    editor_manager->del();

    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}

bool editor::frame()
{
    Uint64 tick_count = SDL_GetTicks();
    Uint64 ticks = tick_count - total_ticks;

    bool count = ticks > fpsticks;

    ImGuiID dock_id = 1;

    if (count)
    {
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        update_run_data();

        view_events();
        
        total_ticks = tick_count;

        if (queued_texture)
        {
            try
            {
                queued_texture->set_sprite(queued_str);

                queued_texture->update_sprite_data();
            }

            catch (...)
            {
                std::cout << "Bad Texture Location!!! LOC:" << queued_str << std::endl;
            }

            queued_texture = nullptr;
        }

        run_processes();

        editor_manager->_process();

        pos size_pos = game_window->get_real_size();
        ImVec2 size = {(float)size_pos.x, (float)size_pos.y};

        ImGui::DockSpaceOverViewport(0, nullptr, ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoDockingOverCentralNode);

        ImGui::SetNextWindowSize(size);
        ImGui::SetNextWindowPos({0.f, 0.f});

        const static constexpr ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar |
                        ImGuiWindowFlags_NoBackground |
                        ImGuiWindowFlags_NoResize |
                        ImGuiWindowFlags_NoMove |
                        ImGuiWindowFlags_NoScrollbar |
                        ImGuiWindowFlags_NoSavedSettings |
                        ImGuiWindowFlags_NoBringToFrontOnFocus;

        ImGui::Begin("##HiddenWindow", nullptr, window_flags);

        ImGui::Dummy(size);
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(draggable_id))
            {
                if (payload->IsDelivery())
                {
                    if (editor_root)
                    {
                        EditorObj* obj = new EditorObj(*(hash*)(payload->Data));
                        if (current_selection)
                        {
                            current_selection->add_child(obj);
                        }
                        
                        else 
                        {
                            editor_root->add_child(obj);
                        }

                        obj->get_tfm()->set(mouse.position.round());

                        obj->set_depth(DRAW_LAYERS::E_OBJ);

                        current_selection = obj;
                    }

                    else 
                    {
                        editor_root = new EditorObj(*(hash*)(payload->Data));
                        editor_manager->add_child(editor_root);

                        editor_root->set_position(mouse.position.round());

                        editor_root->set_depth(DRAW_LAYERS::E_OBJ);

                        current_selection = editor_root;
                    }
                }
            }
            ImGui::EndDragDropTarget();
        }

        if (ImGui::IsItemHovered())
        {
            ImGui::SetNextFrameWantCaptureKeyboard(false);
            ImGui::SetNextFrameWantCaptureMouse(false);
        }

        ImGui::End();

        ImGui::Begin(objects, nullptr);
        if (editor_root)
        {
            editor_root->list_objects(current_selection);
        }
        ImGui::End();

        ImGui::Begin(data, nullptr);
        if (current_selection)
        {
            current_selection->expansion->use_editor();
        }
        ImGui::End();

        ImGui::Begin(create, nullptr);
        show_loadable_processes();
        ImGui::End();

        game_window->prepare_screen();

        // TODO gpu shader, not this mess
        double large_scale = game_window->get_scale().large();
        if (large_scale > scale_too_small)
        {
            SDL_SetTextureAlphaModFloat(line_cube, large_scale - scale_too_small);
            SDL_FRect a;

            pos scaled_cube = cube_size * game_window->get_scale();

            pos modulo = -(game_window->get_center() - game_window->get_half_size()).modulo(cube_size) * game_window->get_scale();

            a.x = modulo.x;
            a.y = modulo.y;
            if (modulo.x > 0) // Inverse : negative
            {
                a.x -= scaled_cube.x;
            }
            if (modulo.y > 0)
            {
                a.y -= scaled_cube.y;
            }

            a.w = game_window->get_real_size().x + scaled_cube.x;
            a.h = game_window->get_real_size().y + scaled_cube.y;
            
            SDL_RenderTextureTiled(game_window->get_renderer(), line_cube, nullptr, large_scale, &a);
        }

        SDL_FRect plus_rect_x;
        plus_rect_x.x = 0;
        plus_rect_x.y = game_window->get_real_half_size().y - game_window->get_center().y * large_scale - large_scale / 2.;
        plus_rect_x.w = game_window->get_real_size().x;
        plus_rect_x.h = std::max(1.,large_scale);
        SDL_RenderTexture(game_window->get_renderer(), center_plus_x, nullptr, &plus_rect_x);

        SDL_FRect plus_rect_y;
        plus_rect_y.x = game_window->get_real_half_size().x - game_window->get_center().x * large_scale - large_scale / 2.;
        plus_rect_y.y = 0;
        plus_rect_y.w = plus_rect_x.h;
        plus_rect_y.h = game_window->get_real_size().y;
        SDL_RenderTexture(game_window->get_renderer(), center_plus_y, nullptr, &plus_rect_y);

        draw();

        ImGui::Render();
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), game_window->get_renderer());

        game_window->push_screen();

        total_frame_ticks = tick_count;

        end_delete();
    }

    else
    {
        SDL_Delay(fpsticks - ticks);
    }

    return running;
}
void display_loadable(const char* str, hash* class_type)
{
    ImGui::Text("%s", str);

    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID | ImGuiDragDropFlags_AcceptNoDrawDefaultRect))
    {
        ImGui::SetDragDropPayload(draggable_id, class_type, sizeof(hash));

        ImGui::Text("%s", str);

        ImGui::EndDragDropSource();
    }
}

void editor::show_loadable_processes()
{
    for (auto x : base_loads_process)
    {
        if (x.second)
        {
            display_loadable(x.second->var_name.c_str(), &(x.second->class_type));
        }
    }  
    
    for (auto y : loadable_processes)
    {
        if (y.second)
        {
            display_loadable(y.second->var_name.c_str(), &(y.second->class_type));
        }
    }
}