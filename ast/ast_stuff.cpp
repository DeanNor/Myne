
#include "ast/ast_stuff.hpp"
#include "ast/ast_load.hpp"
#include "editorobj.hpp"

ast_value* LOAD::create_load(LOAD::load_t v, const std::string& var_name)
{
    switch (v)
    {
    case ERR:
        return nullptr;
    case I:
        return new ast_value_i(var_name);
    case UI:
        return new ast_value_ui(var_name);
    case L:
        return new ast_value_l(var_name);
    case UL:
        return new ast_value_ul(var_name);
    case ULL:
        return new ast_value_ull(var_name);
    case ULI:
        return new ast_value_uli(var_name);
    case C:
        return new ast_value_c(var_name);
    case UC:
        return new ast_value_uc(var_name);
    case B:
        return new ast_value_b(var_name);
    case D:
        return new ast_value_d(var_name);
    case LD:
        return new ast_value_ld(var_name);
    }
}

void load_v::copy_to(EditorObj* root, ast_expansion* parent)
{
    parent->values.push_back(LOAD::create_load(load_enum, var_name));
}

void load_expandable::copy_to(EditorObj* root, ast_expansion* parent)
{
    ast_expansion* copied = new ast_expansion(var_name, class_type, is_class);

    for (auto v : values)
    {
        v->copy_to(root, copied);
    }
    parent->values.push_back(copied);

    if (parent_expansion)
    {
        copied->parent = parent_expansion->copy(root);
    }
}

ast_expansion_base* load_expandable::copy(EditorObj* object)
{
    ast_expansion* copied = new ast_expansion(var_name, class_type, is_class);

    for (auto v : values)
    {
        v->copy_to(object, copied);
    }

    if (parent_expansion)
    {
        copied->parent = parent_expansion->copy(object);
    }

    return copied;
}

EditorObj* find_loaded_process(json& internal)
{
    load_expandable_base* expandable = get_expandable_process(hash(internal["Type"].get<std::string>().c_str()).value);

    EditorObj* edit = new EditorObj;

    edit->set_depth(DRAW_LAYERS::E_OBJ);

    edit->expansion = expandable->copy(edit);

    edit->expansion->load_readable(internal);

    return edit;
}

ast_value* load_from_text(const std::istream& os, ast_expanded* root)
{
    return nullptr;
}

ast_expanded* load_from_file(std::string file_name)
{
    return nullptr; // TODO heavy todo :<
}