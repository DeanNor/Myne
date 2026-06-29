
#include "editorobj.hpp"
#include "click.hpp"
#include "editorstuff.hpp"

void DragObj::on_drag_callback(void* callee, void* caller)
{
    ((editor*)caller)->set_dragged((DragObj*)callee);
}

void EditorObj::on_drag_callback(void* callee, void* caller)
{
    if (((editor*)caller)->get_dragged())
    {
        if (((editor*)caller)->get_dragged()->is_extended_parent_of((Process*)callee)) // Prioritize the child obj
        {
            get_editor()->set_current_selection((EditorObj*)callee);
            ((editor*)caller)->set_dragged((EditorObj*)callee);
        }
    }

    else
    {
        get_editor()->set_current_selection((EditorObj*)callee);
        ((editor*)caller)->set_dragged((EditorObj*)callee);
    }
}

void DragObj::add_to_clicks(ClickManager* manager)
{
    manager->add_to_clicks(&click_managed);
}

void DragObj::remove_from_clicks()
{
    get_editor()->get_click_manager()->remove_from_clicks(&click_managed);
}