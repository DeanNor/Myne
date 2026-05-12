
#pragma once

#include ".hpp/drawobj.hpp"
#include "ast/ast_load.hpp"
#include "ast/ast_stuff.hpp"
#include "click.hpp"
#include "editorstuff.hpp"
#include "imgui.h"

#include "edit.hpp"
#include <stdexcept>

namespace DRAW_LAYERS
{
    enum draw_layer
    {
        E_OBJ,
    };
}

const static ImGuiTreeNodeFlags TREE_NODE_FLAGS = ImGuiTreeNodeFlags_DrawLinesToNodes | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnDoubleClick;

struct ast_process;
struct ast_object;
struct ast_drawobj;

class EditorObj : public DrawObj
{
friend ast_process;
friend ast_object;
friend ast_drawobj;

private:
    bool has_transform = false;

    static void on_drag_callback(void* callee, void* caller);

    void add_to_clicks();
    void remove_from_clicks();

public:
    ast_expansion_base* expansion = nullptr;

    click_rect click_managed;

    EditorObj() : click_managed(&global_transform, EDIT::positional_half_size, {0,0}, on_drag_callback, this, true)
    {
        add_to_clicks();
    }

    EditorObj(hash class_value) : click_managed(&global_transform, EDIT::positional_half_size, {0,0}, on_drag_callback, this, true)
    {
        load_expandable_base* expandable = get_expandable_process(class_value.value);

        if (expandable)
        {
            expansion = expandable->copy(this);
        }

        else throw std::logic_error("Bad class value!!!");

        add_to_clicks();
    }

    ~EditorObj()
    {
        remove_from_clicks();
    }

    void list_objects(EditorObj* current_selection)
    {
        if (current_selection == this)
        {
            ImGui::PushStyleColor(ImGuiCol_Text, 0xFFE8B26Bu); // aabbggrr ???
        }

        if (ImGui::TreeNodeEx(this, TREE_NODE_FLAGS, "%s", name.c_str()))
        {
            if (current_selection == this)
            {
                ImGui::PopStyleColor();
            }

            if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
            {
                get_editor()->set_current_selection(this);
            }

            for (Process* child : children)
            {
                ((EditorObj*)child)->list_objects(current_selection);
            }

            ImGui::TreePop();
        }

        else
        {
            if (current_selection == this)
            {
                ImGui::PopStyleColor();
            }

            if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
            {
                get_editor()->set_current_selection(this);
            }
        }

    }

    tfm* get_tfm_of_expanded()
    {
        if (has_transform)
        {
            return &global_transform;
        }

        return nullptr;
    }

    virtual void set_parent(Process* new_parent) override
    {
        Process::set_parent(new_parent);

        if (has_transform)
        {
            if (EditorObj* par = dynamic_cast<EditorObj*>(new_parent))
            {
                global_transform.parent = par->get_tfm_of_expanded();
            }

            else global_transform.deparent();
        }

        else global_transform.deparent();
    }

    // Call upon setting the sprite value from editor window
    void update_sprite_data()
    {
        if (texture)
        {
            click_managed.half_size = texture->half_size * scale;
            click_managed.scales = false;
        }
        
        else
        {
            click_managed.half_size = EDIT::positional_half_size * scale;
            click_managed.scales = true;
        }
    }

    bool is_extended_parent_of(Process* who)
    {
        if (who->get_parent())
        {
            if (who->get_parent() == this)
            {
                return true;
            }

            else return is_extended_parent_of(who->get_parent());
        }

        else return false;
    }
};