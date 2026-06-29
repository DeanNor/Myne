
#include "editorstuff.hpp"

#include ".hpp/display.hpp"
#include ".hpp/game.hpp"
#include ".hpp/hull.hpp"
#include ".hpp/pos.hpp"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_timer.h"
#include "ast/ast_stuff.hpp"
#include "convex_decomposition/src/ConcavePolygon.h"
#include "edit.hpp"
#include "editorobj.hpp"
#include "imgui.h"
#include <SDL3/SDL_rect.h>
#include <cstddef>
#include <cstdlib>
#include <iterator>

const char* draggable_id = "draggable";

editor::~editor()
{
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    keyboard.remove_watcher(&zoom_speed_watcher);
    keyboard.remove_watcher(&delete_watcher);
}

void editor::normal_mode()
{
    if (dragged)
    {
        if (mouse.ljust_released)
        {
            dragged = nullptr;
        }

        else dragged->get_tfm()->set(mouse.position.round());
    }

    if (mouse.ljust_down)
    {
        click_manager.handle_click(mouse.position, this, game_window->get_scale());
    }
}

void editor::hull_mode()
{
    if (dragged)
    {
        if (mouse.ljust_released)
        {
            dragged = nullptr;
        }

        else
        {
            if (mouse.position.round() != dragged->get_position())
            {
                dragged->set_position(mouse.position.round());

                auto loc = std::find(mode_nodes.begin(), mode_nodes.end(), dragged);
                if (loc != mode_nodes.end())
                {
                    ((hull*)mode_data)->values.setPoint(std::distance(mode_nodes.begin(), loc), cxd::Vec2{(float)dragged->get_position().x, (float)dragged->get_position().y});
                }

                update_hull();
            }
        }
    }

    if (mouse.ljust_down)
    {
        mode_clicks.handle_click(mouse.position, this, game_window->get_scale());

        if (!mode_clicks.was_hit())
        {
            cxd::Vec2 point = {(float)std::round(mouse.position.x), (float)std::round(mouse.position.y)};
            ((hull*)mode_data)->values.addPoint(point);
            HullObj* node = new HullObj();
            node->set_position({point.x, point.y});
            mode_nodes.push_back(node);
            node->set_sprite(EDIT::hull_point, false);
            node->set_depth(100);

            node->add_to_clicks(&mode_clicks);
        }

        update_hull();
    }

    if (((hull*)mode_data)->error == false)
    {
        for (auto x : ((hull*)mode_data)->values.getSubPolygons())
        {
            SDL_SetRenderDrawColor(game_window->get_renderer(), 0xFF, 0xFF, 0xFF, 0xFF);

            SDL_FPoint* lines = (SDL_FPoint*)std::malloc(sizeof(SDL_FPoint) * (x.getVertices().size() + 1));
            for (size_t y = 0; y < x.getVertices().size(); ++y)
            {
                cxd::Vertex v = x.getVertices().at(y);
                lines[y] = {(float)((v.position.x - game_window->get_top_left().x) * game_window->get_scale().x), (float)((v.position.y - game_window->get_top_left().y) * game_window->get_scale().y)};
            }

            lines[x.getVertices().size()] = lines[0];

            SDL_RenderLines(game_window->get_renderer(), lines, x.getVertices().size() + 1);

            free(lines);
        }
    }

    for (auto x : ((hull*)mode_data)->values.getVertices())
    {
        std::cout << x.position.x << ' ' << x.position.y << '\n';
    }
}

void editor::update_hull()
{
    if (((hull*)mode_data)->values.getVertices().size() >= 3)
    {
        *((hull*)mode_data) = ((hull*)mode_data)->values.getVertices();
        ((hull*)mode_data)->decompose_points();
    }
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

        kill_audio_streams();

        if (mouse.mmoved_y)
        {
            pos past_size = game_window->get_size();

            pos past_scale = game_window->get_scale();
            pos changed_scale;
            
            if (mouse.mspeed.y > 0)
            {
                changed_scale = past_scale * (zoom_speed_watcher.down() ? rescale_fast_scale : rescale_scale);
            }

            else
            {
                changed_scale = past_scale / (zoom_speed_watcher.down() ? rescale_fast_scale : rescale_scale);
            }

            game_window->set_scale(changed_scale);

            pos mse_center = mouse.global_position - game_window->get_real_half_size();
            game_window->set_center(game_window->get_center() + ((mse_center / past_scale) - (mse_center / changed_scale)));
        }

        if (mouse.rjust_down)
        {
            move_drag_start = mouse.global_position;
            move_drag_center = game_window->get_center();
            move_dragged = true;
        }

        if (move_dragged)
        {
            game_window->set_center(move_drag_center - (mouse.global_position - move_drag_start) / game_window->get_scale());

            if (mouse.rjust_released)
            {
                move_dragged = false;
            }
        }

        if (delete_watcher.just_pressed())
        {
            if (current_selection)
            {
                current_selection->start_delete();

                if (get_editor_root() == current_selection)
                {
                    set_editor_root(nullptr);
                }

                set_current_selection(nullptr);
            }
        }

        game_window->prepare_screen();

        switch (mode)
        {
        case NORMAL:
            normal_mode();
            break;
        case HULL:
            hull_mode();
            break;
        }
        
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

                        obj->add_to_clicks(&click_manager);

                        obj->set_depth(DRAW_LAYERS::E_OBJ);

                        set_current_selection(obj);
                    }

                    else 
                    {
                        editor_root = new EditorObj(*(hash*)(payload->Data));
                        root->add_child(editor_root);

                        editor_root->set_position(mouse.position.round());

                        editor_root->add_to_clicks(&click_manager);

                        editor_root->set_depth(DRAW_LAYERS::E_OBJ);

                        set_current_selection(editor_root);
                    }

                    enter_normal_mode();
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

        // TODO gpu shader, not this mess
        double large_scale = game_window->get_scale().large();
        float draw_scale = std::min(1.,large_scale);
        if (large_scale > scale_too_small)
        {
            SDL_SetRenderDrawColorFloat(game_window->get_renderer(), 1, 1, 1, draw_scale - scale_too_small);

            pos tl = game_window->get_top_left();
            pos br = game_window->get_bottom_right();

            pos offset = (game_window->get_center() - game_window->get_half_size()).modulo(cube_size) * large_scale;

            if (offset.x > 0)
            {
                offset.x += cube_size.x * large_scale;
            }

            if (offset.y > 0)
            {
                offset.y += cube_size.y * large_scale;
            }

            size_t v_rows = std::ceil((br.x - tl.x) / cube_size.x) + 2;

            for (size_t x = 0; x < v_rows; x++)
            {
                float x_val = large_scale * cube_size.x * x - offset.x;
                SDL_RenderLine(game_window->get_renderer(), x_val,0, x_val, game_window->get_real_size().y);
            }

            size_t h_rows = std::ceil((br.y - tl.y) / cube_size.y) + 2;
            
            for (size_t y = 0; y < h_rows; y++)
            {
                float y_val = large_scale * cube_size.y * y - offset.y;
                SDL_RenderLine(game_window->get_renderer(), 0,y_val, game_window->get_real_size().x,y_val);
            }
        }

        { // Render Center Plus X
            float y = -game_window->get_center().y * large_scale + game_window->get_real_half_size().y;
            float w = game_window->get_real_size().x;
            SDL_SetRenderDrawColor(game_window->get_renderer(), center_plus_x.r, center_plus_x.g, center_plus_x.b, 0xFF);
            SDL_RenderLine(game_window->get_renderer(), 0, y, w, y);
        }

        { // Render Center Plus Y
            float x = -game_window->get_center().x * large_scale + game_window->get_real_half_size().x;
            float h = game_window->get_real_size().y;

            SDL_SetRenderDrawColor(game_window->get_renderer(), center_plus_y.r, center_plus_y.g, center_plus_y.b, 0xFF);
            SDL_RenderLine(game_window->get_renderer(), x, 0, x, h);
        }

        SDL_SetRenderDrawColor(game_window->get_renderer(), 0, 0, 0, SDL_ALPHA_OPAQUE);

        for (auto x : new_draws) // copied from game::run_frame()
        {
            draws[x.second].push_back(x.first);
        }

        new_draws.clear();

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