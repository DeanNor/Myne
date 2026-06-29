
#pragma once

#include ".hpp/sprite.hpp"
#include "SDL3/SDL_dialog.h"
#include "ast/ast_stuff.hpp"
#include "convex_decomposition/src/ConcavePolygon.h"
#include "edit.hpp"
#include "editorstuff.hpp"
#include "imgui.h"

#include "editorobj.hpp"

#include ".hpp/pos.hpp"
#include <filesystem>
#include <stdexcept>

struct ast_pos : public ast_expansion_base
{
ASSIGN_VIR_VAR_CONSTRUCTOR(ast_pos);

public:
    pos v = {0,0};

    ast_pos() = default;

    ast_pos(std::string _var_name) : ast_expansion_base(_var_name, "pos", false) {}

    virtual void use_editor() override
    {
        ImGui::Text("%s", var_name.c_str());

        _use_editor(&v);
    }

    static void _use_editor(pos* _v)
    {
        ImGui::Text("x");
        ImGui::SameLine();
        ImGui::PushID(&_v->x);
        ImGui::InputDouble("##xx", &_v->x);
        ImGui::PopID();

        ImGui::Text("y");
        ImGui::SameLine();
        ImGui::PushID(&_v->y);
        ImGui::InputDouble("##xx", &_v->y);
        ImGui::PopID();
    }

    virtual void save(Saver* saver) const override
    {
        saver->save_complex(v);
    }

    virtual void save_readable(json& os) const override
    {
        json internal = _save_readable(&v);

        os[var_name] = internal;
    }

    static json _save_readable(const pos* _v)
    {
        json chunk;

        chunk["Type"] = "pos";

        chunk["x"] = JSON(double, _v->x);

        chunk["y"] = JSON(double, _v->y);

        return chunk;
    }

    virtual void load_readable(json& os) override
    {
        json internal = os[var_name];

        v = _load_readable(internal);
    }

    static pos _load_readable(json& _os)
    {
        pos v;

        // TODO check type function
        if (_os["Type"].get<std::string>() == "pos")
        {
            v.x = _os["x"]["v"].get<double>();
            v.y = _os["y"]["v"].get<double>();
        }

        else throw std::logic_error("!");

        return v;
    }
};

#include ".hpp/rad.hpp"
struct ast_rad : public ast_expansion_base
{
ASSIGN_VIR_VAR_CONSTRUCTOR(ast_rad);

public:
    rad v{0.};

    int degree_v;

    static constexpr int angle_high = 360;
    static constexpr int angle_low = 0;

    ast_rad() = default;

    ast_rad(std::string _var_name) : ast_expansion_base(_var_name, "rad", false) {}

    virtual void use_editor() override
    {
        ImGui::Text("%s", var_name.c_str());

        ImGui::Text("radian");
        ImGui::SameLine();

        _use_editor(&v,&degree_v);
    }

    static void _use_editor(rad* _v, int* _degree_v)
    {
        ImGui::PushID(_v);
        if (ImGui::InputDouble("##xx",&(_v->radian))) // If changed
        {
            *_v = rad::_constrain_rad(*_v);
            *_degree_v = _v->deg();
        }
        ImGui::PopID();

        ImGui::PushID(_degree_v);
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::SliderInt("##xx", _degree_v, ast_rad::angle_low, ast_rad::angle_high))
        {
            *_v = rad::force(rad::_constrain_rad(*_degree_v / rad::DEG_CONV_CONST()));
        }
        ImGui::PopID();
    }

    virtual void save(Saver* saver) const override
    {
        saver->save_complex(v);
    }

    virtual void save_readable(json& os) const override
    {
        json internal = _save_readable(&v);

        os[var_name] = internal;
    }

    static json _save_readable(const rad* _v)
    {
        json internal;

        internal["Type"] = "rad";

        internal["radian"] = JSON(double, _v->radian);

        return internal;
    }

    virtual void load_readable(json& os) override
    {
        json internal = os[var_name];

        v = _load_readable(internal);

        degree_v = v.deg(); // TODO not separate values!!!
    }

    static rad _load_readable(json& internal)
    {
        rad _v;

        if (internal["Type"].get<std::string>() == "rad")
        {
            _v = rad::force(internal["radian"]["v"].get<double>());
        }

        else throw std::logic_error("!");

        return _v;
    }
};

#include "imgui-docking/misc/cpp/imgui_stdlib.h"

struct ast_string : public ast_expansion_base
{
ASSIGN_VIR_VAR_CONSTRUCTOR(ast_string);
    
public:
    std::string v;

    ast_string() = default;

    ast_string(std::string _var_name) : ast_expansion_base(_var_name, "std::string", false) {}

    virtual void use_editor() override
    {
        ImGui::Text("%s", var_name.c_str());
        ImGui::SameLine();

        _use_editor(&v);
    }

    static void _use_editor(std::string* _v)
    {
        ImGui::PushID(_v);
        ImGui::InputText("##xx", _v);
        ImGui::PopID();
    }

    virtual void save(Saver* saver) const override
    {
        saver->save_complex(v);
    }

    virtual void save_readable(json& os) const override
    {
        json internal = _save_readable(&v);

        os[var_name] = internal;
    }

    static json _save_readable(const std::string* _v)
    {
        json internal = JSON(std::string, *_v);

        return internal;
    }

    virtual void load_readable(json& os) override
    {
        json internal = os[var_name];

        v = _load_readable(internal);
    }

    static std::string _load_readable(json& internal)
    {
        std::string _v;

        if (internal["Type"].get<std::string>() == "std::string")
        {
            _v = internal["v"].get<std::string>();
        }

        return _v;
    }
};

#include ".hpp/hull.hpp"

#include "editorstuff.hpp"

struct ast_hull : public ast_expansion_base
{
public:
    std::filesystem::path v_path;

    std::pair<hull,std::vector<cxd::Vertex>> v;

    bool active;

    ast_hull() = default;

    ast_hull(std::string _var_name) : ast_expansion_base(_var_name, "hull", false) {}

    ~ast_hull()
    {
        if (active)
        {
            get_editor()->enter_normal_mode();
        }
    }

    virtual void use_editor() override
    {
        ImGui::Text("%s", var_name.c_str());
        ImGui::SameLine();

        _use_editor(&v_path, &v, &active);
    }

    static void _use_editor(std::filesystem::path* _v_path, std::pair<hull,std::vector<cxd::Vertex>>* _v, bool* active_check)
    {
        ImGui::PushID(_v_path);

        if (std::filesystem::exists(*_v_path))
        {
            get_editor()->enter_hull_save_mode(_v, active_check);
        }

        ImGui::PopID();
    }

    virtual void save(Saver* saver) const override
    {
        saver->save_complex(v.first);
    }

    virtual void save_readable(json& os) const override
    {
        json internal = _save_readable(&v_path);

        os[var_name] = internal;

        Saver file(v_path);
        
        file.save_complex(hull(v.second));

        // TODO final compile version that loads, decomposes, and saves separately
    }

    static json _save_readable(const std::filesystem::path* _v)
    {
        json internal = JSON(std::filesystem::path, *_v);

        return internal;
    }

    virtual void load_readable(json& os) override
    {
        json internal = os[var_name];

        v_path = _load_readable(internal);
    }

    static std::filesystem::path _load_readable(json& internal)
    {
        std::string _v;

        if (internal["Type"].get<std::string>() == "std::string")
        {
            _v = internal["v"].get<std::string>();
        }

        return _v;
    }
};

// TODO error if unable to enable specific file_filter stuff (can this be autoloaded from CMAKE data???)
const constexpr inline SDL_DialogFileFilter file_filter{"Select Image", "stb;avif;bmp;gif;jpg;lbm;pcx;png;pnm;qoi;svg;tga;tif;webp;xcf;xpm;xv;imageio;wic;jxl"};

#include "SDL3/SDL_render.h"
#include ".hpp/SDL3.h"

struct ast_sprite : public ast_expansion_base
{
ASSIGN_VIR_VAR_CONSTRUCTOR(ast_sprite);

private:
    // Assumes only 1 file and that texture_obj is an ast_sprite
    static void get_file_callback(void * texture_obj, const char* const* file, int num_files) // TODO ERRORS
    {
        try
        {
            SDL_Texture* v = load_img(get_current_game()->get_game_window()->get_renderer(), file[0]);
            ((ast_sprite*)texture_obj)->texture->set(v);
        }

        catch (std::invalid_argument err)
        {
            // TODO log error
            std::cout << err.what() << std::endl;
        }
    }

public:
    basic_sprite* texture;

    ast_sprite() = default;

    ast_sprite(std::string _var_name) : ast_expansion_base(_var_name, "basic_sprite*", false) {}

    virtual void use_editor() override
    {
        //if (*texture) ImGui::Image((ImTextureID)*texture, {(float)(*texture)->w, (float)(*texture)->h});

        if (ImGui::Button(var_name.data()))
        {
            SDL_ShowOpenFileDialog(get_file_callback, this, get_current_game()->get_game_window()->get_window(), &file_filter , 1, "/", false);
        }
    }

    virtual void save(Saver* saver) const override
    {

    }

    static json _save_readable(basic_sprite* v)
    {
        json internal;
        internal["Path"] = v->sprite_path;

        return internal;
    }

    static json _save_readable(SDL_Texture* v, std::filesystem::path path)
    {
        json internal;
        internal["Path"] = path;

        return internal;
    }

    virtual void save_readable(json& os) const override
    {
        json internal = _save_readable(texture);

        os[var_name] = internal;
    }

    static basic_sprite* _load_readable(json& internal)
    {
        // TODO of each type :(
        return nullptr;
    }

    virtual void load_readable(json& os) override
    {

    }
};

struct ast_process : public ast_expansion_base
{
ASSIGN_VIR_VAR_CONSTRUCTOR(ast_process);

public:
    EditorObj* owner_v;

    ast_process() = default;

    ast_process(std::string _var_name) : ast_expansion_base(_var_name, "Process", true) {}

    void fill(EditorObj* owner)
    {
        owner_v = owner;
    }

    virtual void use_editor() override
    {
        ImGui::Text("%s", var_name.c_str());

        ImGui::Text("%s", "name");
        ImGui::SameLine();
        ast_string::_use_editor(&owner_v->name);
    }

    virtual void save(Saver* saver) const override
    {
        
    }

    virtual void save_readable(json& os) const override
    {
        json internal;

        internal["Type"] = "Process";

        internal["name"] = ast_string::_save_readable(&owner_v->name);

        internal["position"] = ast_pos::_save_readable(&owner_v->position);

        json children_json;

        for (Process* x : owner_v->children)
        {
            ((EditorObj*)x)->expansion->save_readable(children_json);
        }

        internal["Children Chunk"] = children_json;

        os.push_back(internal);
    }

    virtual void load_readable(json& internal) override
    {
        owner_v->set_name(ast_string::_load_readable(internal["name"]));
        owner_v->set_position(ast_pos::_load_readable(internal["position"]));

        for (auto x : internal["Children Chunk"])
        {
            owner_v->add_child(find_loaded_process(x));
        }
    }
};

struct ast_object : public ast_expansion_base
{
ASSIGN_VIR_VAR_CONSTRUCTOR(ast_object);

public:
    pos* pos_v;
    rad* rad_v;

    int degree_v = 0;

    ast_object() = default;

    ast_object(std::string _var_name) : ast_expansion_base(_var_name, "Object", true) {}

    void fill(EditorObj* owner)
    {
        ((ast_process*)parent)->fill(owner);

        pos_v = &owner->position;

        rad_v = &owner->angle;

        degree_v = rad_v->deg();

        owner->has_transform = true;
    }

    virtual void use_editor() override
    {
        ImGui::Text("%s", var_name.c_str());

        ImGui::Text("%s", "position");
        ast_pos::_use_editor(pos_v);

        ImGui::Text("%s", "angle");
        ImGui::Text("radian");
        ImGui::SameLine();
        ast_rad::_use_editor(rad_v, &degree_v);

        parent->use_editor();
    }

    virtual void save(Saver* saver) const override
    {

    }

    virtual void save_readable(json& os) const override
    {
        json internal;

        json process_chunk;

        parent->save_readable(process_chunk);

        internal["Type"] = "Object";

        internal["angle"] = ast_rad::_save_readable(rad_v);

        internal["Process Chunk"] = process_chunk[0]; // TODO remove [0] workaround

        os.push_back(internal);
    }

    virtual void load_readable(json& internal) override
    {
        *rad_v = ast_rad::_load_readable(internal["angle"]);

        degree_v = rad_v->deg();

        parent->load_readable(internal["Process Chunk"]);
    }
};

struct ast_drawobj : ast_expansion_base
{
ASSIGN_VIR_VAR_CONSTRUCTOR(ast_drawobj);

private:
    static void open_sprite_callback(void* drawobj, const char* const* files, int file_count)
    {
        if (files && file_count == 1)
        {
            get_editor()->set_loaded_img(drawobj, files[0]);
        }
    }

public:
    DrawObj* drawer;

    ast_drawobj() = default;

    unsigned char depth = 0;

    ast_drawobj(std::string _var_name) : ast_expansion_base(_var_name, "DrawObj", true) {}

    void fill(EditorObj* owner)
    {
        ((ast_object*)parent)->fill(owner);

        drawer = owner;
    }

    virtual void use_editor() override
    {
        ImGui::Text("%s", var_name.c_str());

        // TODO use texture:: version of this
        if (ImGui::Button("change sprite"))
        {
            SDL_ShowOpenFileDialog(open_sprite_callback, drawer, get_current_game()->get_game_window()->get_window(), &file_filter , 1, "/", false);
        }

        ImGui::Text("draw depth");
        ImGui::PushID(this);
        if (ImGui::InputScalar("##xx", ImGuiDataType_U8, &depth))
        {
            drawer->set_depth(depth);
        }
        ImGui::PopID();

        parent->use_editor();
    }

    virtual void save(Saver* saver) const override
    {

    }

    virtual void save_readable(json& os) const override
    {
        json internal;

        json object_chunk;

        parent->save_readable(object_chunk);

        internal["Type"] = "DrawObj";

        if (drawer->texture)
        {
            internal["texture"] = ast_sprite::_save_readable(drawer->texture);
        }

        else
        {
            internal["texture"] = ast_sprite::_save_readable(EDIT::basic_positional->get(), "NULL");
        }

        internal["Object Chunk"] = object_chunk[0]; // TODO remove [0] workaround

        os.push_back(internal);
    }

    virtual void load_readable(json& internal) override
    {
        drawer->texture = ast_sprite::_load_readable(internal["texture"]);

        parent->load_readable(internal["Object Chunk"]);
    }
};






























































struct load_pos : public load_expandable_base
{
public:
    load_pos(std::string _var_name) : load_expandable_base(_var_name, "pos") {is_class = false;}

    virtual void copy_to(EditorObj*, ast_expansion* parent) override
    {
        parent->values.push_back(new ast_pos(var_name));
    }
};

struct load_rad : public load_expandable_base
{
public:
    load_rad(std::string _var_name) : load_expandable_base(_var_name, "rad") {is_class = false;}

    virtual void copy_to(EditorObj*, ast_expansion* parent) override
    {
        parent->values.push_back(new ast_rad(var_name));
    }
};

struct load_string : public load_expandable_base
{
public:
    load_string(std::string _var_name) : load_expandable_base(_var_name, "std::string") {is_class = false;}

    virtual void copy_to(EditorObj*, ast_expansion* parent) override
    {
        parent->values.push_back(new ast_string(var_name));
    }
};

struct load_texture : public load_expandable_base
{
public:
    load_texture(std::string _var_name) : load_expandable_base(_var_name, "basic_sprite*") {is_class = false;}

    virtual void copy_to(EditorObj*, ast_expansion* parent) override
    {
        parent->values.push_back(new ast_sprite(var_name));
    }
};

struct load_process : public load_expandable_base
{
public:
    load_process(std::string _var_name) : load_expandable_base(_var_name, "Process") {is_class = true;}

    virtual ast_expanded* copy(EditorObj* owner) override
    {
        ast_process* process = new ast_process(var_name);

        process->fill(owner);

        return (ast_expanded*)process;
    }
};

struct load_object : public load_expandable_base
{
public:
    load_object(std::string _var_name) : load_expandable_base(_var_name, "Object")
    {
        is_class = true;
        parent_expansion = base_loads_process.at(hash("Process").value);
    }

    virtual ast_expansion_base* copy(EditorObj* owner) override
    {
        ast_object* object = new ast_object(var_name);

        object->parent = parent_expansion->copy(owner);

        object->fill(owner);

        return object;
    }
};

struct load_drawobj : public load_expandable_base
{
public:
    load_drawobj(std::string _var_name) : load_expandable_base(_var_name, "DrawObj")
    {
        is_class = false;

        parent_expansion = base_loads_process.at(hash("Object").value);
    }

    virtual ast_expansion_base* copy(EditorObj* owner) override
    {
        ast_drawobj* drawobj = new ast_drawobj(var_name);

        drawobj->parent = parent_expansion->copy(owner);

        drawobj->fill(owner);

        return drawobj;
    }
};