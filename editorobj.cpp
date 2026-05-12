
#include "editorobj.hpp"
#include "editormanager.hpp"
#include "editorstuff.hpp"

void EditorObj::on_drag_callback(void* callee, void* caller)
{
    if (((EditorManager*)caller)->dragged)
    {
        if (((EditorManager*)caller)->dragged->is_extended_parent_of((Process*)callee)) // Prioritize the child obj
        {
            ((EditorManager*)caller)->dragged = (EditorObj*)callee;
            get_editor()->set_current_selection((EditorObj*)callee);
        }
    }

    else
    {
        ((EditorManager*)caller)->dragged = (EditorObj*)callee;
        get_editor()->set_current_selection((EditorObj*)callee);
    }
}

void EditorObj::add_to_clicks()
{
    get_editor()->get_editor_manager()->click_manager.add_to_clicks(&click_managed);
}

void EditorObj::remove_from_clicks()
{
    get_editor()->get_editor_manager()->click_manager.remove_from_clicks(&click_managed);
}