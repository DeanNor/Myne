
#pragma once

#include ".hpp/factory.hpp"
#include ".hpp/hash.hpp"
#include ".hpp/saver.hpp"
#include <string>
#include <vector>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <imgui-docking/imgui.h>

struct load_expandable_base;

// TODO list of inheritances to show in editor
inline std::unordered_map<hash_t, load_expandable_base*> loadable_processes;
inline std::unordered_map<hash_t, load_expandable_base*> loadable_complexes;

inline std::unordered_map<hash_t, load_expandable_base*> base_loads_process;
inline std::unordered_map<hash_t, load_expandable_base*> base_loads_complex;

struct ast_value;
struct ast_expanded;
ast_value* load_from_text(const json& os);

ast_expanded* load_from_file(std::string file_name);

// When creating a custom, have the loadable give the knowledge to an expanded, and just add the child to the parent. Use no expansion in the loaded val

namespace EXPANSION_TYPE
{
    enum expand_t
    {
        ERR,
        PROCESS,
        COMPLEX,
        DATA,
        ENUM,
    };
}

template <typename T>
constexpr ImGuiDataType get_data_type()
{
    // TODO sizeof(T)
    if (std::is_signed_v<T>)
    {
        switch (sizeof(T))
        {
        case 16:
            return ImGuiDataType_Double;
        case 8:
            return ImGuiDataType_S64;
        case 4:
            return ImGuiDataType_S32;
        case 2:
            return ImGuiDataType_S16;
        case 1:
            return ImGuiDataType_S8;
        }
    }

    else
    {
        switch (sizeof(T))
        {
        case 16:
            return ImGuiDataType_Double;
        case 8:
            return ImGuiDataType_U64;
        case 4:
            return ImGuiDataType_U32;
        case 2:
            return ImGuiDataType_U16;
        case 1:
            return ImGuiDataType_U8;
        }
    }
}

struct ast_value;
struct ast_expanded;
struct ast_expansion_base;
struct ast_expansion;

struct class_hint
{
    bool is_class;
    std::string str_name;
    hash class_name;

    bool inherits;
    std::string str_parent;
    hash parent_name;

    class_hint(bool _is_class, std::string name) : is_class(_is_class), str_name(name), class_name(name.data()), inherits(false) {}

    class_hint(bool _is_class, std::string name, std::string parent) : is_class(_is_class), str_name(name), class_name(name.data()), inherits(true), str_parent(parent), parent_name(parent.data()) {}
};

namespace LOAD
{
    enum load_t
    {
        ERR,

        I,
        UI,
        L,
        UL,
        ULL,
        ULI,
        C,
        UC,
        B,
        D,
        LD,
    };

    ast_value* create_load(LOAD::load_t v, const std::string& var_name);

    inline LOAD::load_t get_type(hash type)
    {
        switch (type.value)
        {
        case hash("int").value:
            return I;
        case hash("unsigned int").value:
            return UI;
        case hash("long").value:
            return L;
        case hash("unsigned long").value:
            return UL;
        case hash("unsigned long long").value:
            return ULL;
        case hash("unsigned long int").value:
            return ULI;
        case hash("char").value:
            return C;
        case hash("unsigned char").value:
            return UC;
        case hash("bool").value:
            return B;
        case hash("double").value:
            return D;
        case hash("long double").value:
            return LD;
        }

        return ERR;

        // TODO custom type/enum caching
    }
}

class EditorObj;

struct load_type
{
public:
    std::string var_name; // TODO remove, there is no need except for base classes and parents, so a huge need. A problem indeed

    load_type(std::string _var_name) : var_name(_var_name) {}

    virtual void copy_to(EditorObj* root, ast_expansion* parent) = 0;
};

struct load_v : public load_type
{
protected:
    LOAD::load_t load_enum;

public:
    load_v(std::string _var_name, LOAD::load_t val) : load_type(_var_name), load_enum(val) {}

    virtual void copy_to(EditorObj* root, ast_expansion* parent) override;
};

struct EditorObj;

#include <iostream>

struct load_expandable_base : public load_type
{
public:
    bool is_class;

    hash class_type;
    
    load_expandable_base* parent_expansion = nullptr;

    load_expandable_base(std::string _var_name, hash class_name) : load_type(_var_name), class_type(class_name) {}

    load_expandable_base(std::string _var_name, const char* _class_type) : load_type(_var_name), class_type(_class_type) {}

    virtual void copy_to(EditorObj* root, ast_expansion* parent) {std::cout << var_name << " load_expandable_base::copy_to() called!" << std::endl;}

    virtual ast_expansion_base* copy(EditorObj* owner) {std::cout << var_name << " load_expandable_base::copy() called!" << std::endl; return nullptr;}
};

struct load_expandable : public load_expandable_base
{
public:
    std::vector<load_type*> values;

    load_expandable(std::string _var_name, hash class_name) : load_expandable_base(_var_name, class_name) {}

    virtual void copy_to(EditorObj* root, ast_expansion* parent) override;
    
    virtual ast_expansion_base* copy(EditorObj* owner) override;
};

#include <iostream>
struct load_expands : public load_type
{
public:
    load_expandable_base* expansion = nullptr;

    hash class_type;

    bool is_class;

    load_expands(std::string _var_name, hash class_name) : load_type(_var_name), class_type(class_name) {}

    virtual void copy_to(EditorObj* root, ast_expansion* parent) override
    {
        if (expansion)
        {
            expansion->copy_to(root, parent);
        }
        
        else 
        {
            if (is_class)
            {
                try
                {
                    expansion = loadable_processes.at(class_type.value);
                }

                catch (...)
                {
                    expansion = base_loads_process.at(class_type.value);
                }
            }

            else
            {
                try
                {
                    expansion = loadable_complexes.at(class_type.value);
                }

                catch (...)
                {
                    expansion = base_loads_complex.at(class_type.value);
                }
            }

            if (expansion)
            {
                expansion->var_name = var_name;
                expansion->copy_to(root, parent);
            }

            else
            {
                std::cout << "Error: " << var_name << " does not know what to load from. Link the files for its type's loads" << std::endl;
            }
        }
    }

    ast_expanded* copy();
    
};

#define JSON(type, var) \
json({ \
        {"Type",#type}, \
        {"v",var}, \
}) \

struct ast_value
{
VIR_NAME_TYPE_OVERRIDE(ast_value);

public:
    std::string var_name;

    ast_value() = default;

    ast_value(std::string _var_name) : var_name(_var_name) {}

    virtual void use_editor() = 0;

    virtual void save(Saver* saver) const = 0;

    virtual void save_readable(json& os) const = 0;

    virtual void load_readable(json& os) = 0;
};

struct ast_expansion_base : public ast_value
{
VIR_NAME_TYPE(ast_expansion_base);

public:
    bool is_class;

    hash class_type;

    ast_expansion_base* parent;

    ast_expansion_base() = default;

    ast_expansion_base(std::string _var_name, hash _class_type, bool _is_class) : ast_value(_var_name), class_type(_class_type), is_class(_is_class) {}

    ast_expansion_base(std::string _var_name, const char* _class_type, bool _is_class) : ast_value(_var_name), class_type(_class_type), is_class(_is_class) {}

    virtual void use_editor() override = 0;

    virtual void save(Saver* saver) const override
    {
        saver->save_complex(class_type);
    }
};

EditorObj* find_loaded_process(json& internal);

struct ast_expansion : public ast_expansion_base
{
VIR_NAME_TYPE(ast_expansion);

public:
    std::vector<ast_value*> values;

    ast_expansion() = default;

    ast_expansion(std::string _var_name, hash _class_type, bool is_class) : ast_expansion_base(_var_name, _class_type, is_class) {}

    virtual void use_editor() override
    {
        ImGui::Text("%s", var_name.data());

        for (ast_value* x : values)
        {
            x->use_editor();
        }

        parent->use_editor();
    }

    virtual void save(Saver* saver) const override
    {
        ast_expansion_base::save(saver);

        for (auto x : values)
        {
            x->save(saver);
        }
    }

    static inline std::string get_parent_chunk(std::string v)
    {
        return v + " Chunk";
    }

    virtual void save_readable(json& os) const override
    {
        json internal;
        for (ast_value* x : values)
        {
            x->save_readable(internal);
        }

        if (parent)
        {
            json parent_chunk;
            parent->save_readable(parent_chunk);

            internal[get_parent_chunk(parent->var_name)] = parent_chunk[0]; // TODO remove [0] workaround
        }

        internal["Type"] = var_name;

        os.push_back(internal);
    }

    virtual void load_readable(json& os) override
    {
        for (ast_value* x : values)
        {
            x->load_readable(os);
        }

        if (parent) parent->load_readable(os[ast_expansion::get_parent_chunk(parent->var_name)]);
    }
};

struct ast_expanded : public ast_expansion
{
ASSIGN_VIR_VAR_CONSTRUCTOR(ast_expanded);

public:
    ast_expanded() = default;

    ast_expanded(std::string _var_name, hash _class_type) : ast_expansion(_var_name, _class_type, true) {}

    virtual void use_editor() override
    {
        for (ast_value* x : values)
        {
            x->use_editor();
        }
    }
};

// TODO template all below
struct ast_value_i : public ast_value
{
private:
    int v = 0;
public:
    ast_value_i(std::string _var_name) : ast_value(_var_name) {}

    virtual void use_editor() override 
    {
        ImGui::Text("%s", var_name.c_str());
        ImGui::SameLine();

        ImGui::PushID(this);
        ImGui::InputInt("##xx", &v);
        ImGui::PopID();
    }

    virtual void save(Saver *saver) const override
    {
        saver->save_data(v);
    }

    virtual void save_readable(json& os) const override
    {
        
    }
    
    virtual void load_readable(json& os) override
    {
        
    }
};
struct ast_value_ui : public ast_value
{
private:
    unsigned int v = 0;
public:
    ast_value_ui(std::string _var_name) : ast_value(_var_name) {}

    virtual void use_editor() override
    {
        ImGui::Text("%s", var_name.c_str());
        ImGui::SameLine();

        ImGui::PushID(this);
        ImGui::InputScalar("##xx", get_data_type<unsigned int>(),&v);
        ImGui::PopID();
    }

    void save(Saver *saver) const override
    {
        saver->save_data(v);
    }

    void save_readable(json& os) const override
    {

    }

    virtual void load_readable(json& os) override
    {

    }
};
struct ast_value_l : public ast_value
{
private:
    long v = 0;
public:
    ast_value_l(std::string _var_name) : ast_value(_var_name) {}

    virtual void use_editor() override
    {
        ImGui::Text("%s", var_name.c_str());
        ImGui::SameLine();

        ImGui::PushID(this);
        ImGui::InputScalar("##xx", get_data_type<long>(),&v);
        ImGui::PopID();
    }

    void save(Saver *saver) const override
    {
        saver->save_data(v);
    }

    void save_readable(json& os) const override
    {
        
    }

    virtual void load_readable(json& os) override
    {

    }
};
struct ast_value_ul : public ast_value
{
private:
    unsigned long v = 0;
public:
    ast_value_ul(std::string _var_name) : ast_value(_var_name) {}

    virtual void use_editor() override
    {
        ImGui::Text("%s", var_name.c_str());
        ImGui::SameLine();

        ImGui::PushID(this);
        ImGui::InputScalar("##xx", get_data_type<unsigned long>(),&v);
        ImGui::PopID();
    }

    void save(Saver *saver) const override
    {
        saver->save_data(v);
    }

    void save_readable(json& os) const override
    {

    }

    virtual void load_readable(json& os) override
    {

    }
};
struct ast_value_ull : public ast_value
{
private:
    unsigned long long v = 0;
public:
    ast_value_ull(std::string _var_name) : ast_value(_var_name) {}

    virtual void use_editor() override
    {
        ImGui::Text("%s", var_name.c_str());
        ImGui::SameLine();

        ImGui::PushID(this);
        ImGui::InputScalar("##xx", get_data_type<unsigned long long>(),&v);
        ImGui::PopID();
    }

    void save(Saver *saver) const override
    {
        saver->save_data(v);
    }

    void save_readable(json& os) const override
    {

    }

    virtual void load_readable(json& os) override
    {

    }
};
struct ast_value_uli : public ast_value
{
private:
    unsigned long int v = 0;
public:
    ast_value_uli(std::string _var_name) : ast_value(_var_name) {}

    virtual void use_editor() override
    {
        ImGui::Text("%s", var_name.c_str());
        ImGui::SameLine();

        ImGui::PushID(this);
        ImGui::InputScalar("##xx", get_data_type<unsigned long int>(),&v);
        ImGui::PopID();
    }

    void save(Saver *saver) const override
    {
        saver->save_data(v);
    }


    void save_readable(json& os) const override
    {

    }

    virtual void load_readable(json& os) override
    {

    }
};
struct ast_value_c : public ast_value
{
private:
    char v = 0;
public:
    ast_value_c(std::string _var_name) : ast_value(_var_name) {}

    virtual void use_editor() override
    {
        ImGui::Text("%s", var_name.c_str());
        ImGui::SameLine();

        ImGui::PushID(this);
        ImGui::InputText("##xx", &v, 1);
        ImGui::PopID();
    }

    void save(Saver *saver) const override
    {
        saver->save_data(v);
    }

    void save_readable(json& os) const override
    {

    }

    virtual void load_readable(json& os) override
    {

    }
};
struct ast_value_uc : public ast_value
{
private:
    unsigned char v = 0;
public:
    ast_value_uc(std::string _var_name) : ast_value(_var_name) {}

    virtual void use_editor() override
    {
        ImGui::Text("%s", var_name.c_str());
        ImGui::SameLine();

        ImGui::PushID(this);
        ImGui::InputScalar("##xx", get_data_type<unsigned char>(),&v);
        ImGui::PopID();
    }

    void save(Saver *saver) const override
    {
        saver->save_data(v);
    }

    void save_readable(json& os) const override
    {

    }

    virtual void load_readable(json& os) override
    {

    }
};

struct ast_value_b : public ast_value
{
private:
    bool v = 0;
public:
    ast_value_b(std::string _var_name) : ast_value(_var_name) {}

    virtual void use_editor() override
    {
        ImGui::Text("%s", var_name.c_str());
        ImGui::SameLine();

        ImGui::PushID(this);
        ImGui::Checkbox("##xx", &v);
        ImGui::PopID();
    }

    void save(Saver *saver) const override
    {
        saver->save_data(v);
    }

    void save_readable(json& os) const override
    {

    }

    virtual void load_readable(json& os) override
    {

    }
};
struct ast_value_d : public ast_value
{
private:
    double v = 0;
public:
    ast_value_d(std::string _var_name) : ast_value(_var_name) {}

    virtual void use_editor() override
    {
        ImGui::Text("%s", var_name.c_str());
        ImGui::SameLine();

        ImGui::PushID(this);
        ImGui::InputDouble("##xx", &v);
        ImGui::PopID();
    }

    void save(Saver *saver) const override
    {
        saver->save_data(v);
    }

    void save_readable(json& os) const override
    {
        
    }

    virtual void load_readable(json& os) override
    {

    }
};
struct ast_value_ld : public ast_value
{
private:
    long double v = 0;
public:
    ast_value_ld(std::string _var_name) : ast_value(_var_name) {}

    virtual void use_editor() override
    {
        ImGui::Text("%s", var_name.c_str());
        ImGui::SameLine();

        ImGui::PushID(this);
        ImGui::InputScalar("##xx", get_data_type<long double>(),&v);
        ImGui::PopID();
    }

    void save(Saver *saver) const override
    {
        saver->save_data(v);
    }

    void save_readable(json& os) const override
    {

    }

    virtual void load_readable(json& os) override
    {

    }
};

inline load_type* create_ast(std::string lhs, std::string function_name, hash template_arg)
{
    hash func_hash(function_name.c_str());
    
    switch (func_hash.value)
    {
    case hash("load_complex").value:
    case hash("load_complex_ptr").value:
    case hash("load_process").value:
        return new load_expands(lhs, template_arg);

    case hash("load_data").value:
        return new load_v(lhs, LOAD::get_type(template_arg));

    case hash("load_enum").value:
        // TODO ???
        return nullptr;

    default:
        return nullptr;
    }
}
