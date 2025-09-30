
#pragma once

#include "./.hpp/hash.hpp"

#include "ast/print.hpp"

#include <cstddef>
#include <string>
#include <vector>
#include <mutex>

#include <imgui-docking/imgui.h>

constexpr inline void add_err_block()
{
    ImGui::Text("ERROR UNKNOWN EXPANSION/VALUE");
}

struct load_ast
{
public:
    std::string name;

    virtual void use_editor()
    {
        add_err_block();
    };

    virtual ~load_ast()
    {
        
    }

    load_ast(std::string variable_name) : name(variable_name)
    {

    }

    load_ast() = default;

    virtual load_ast* copy()
    {
        return nullptr;
    }
};


struct named_hash : public hash
{
public:
    std::string name;

    named_hash(const char* to_hash) : hash(to_hash), name(to_hash)
    {

    }

    named_hash(const hash& other) : hash(other)
    {

    }

    named_hash() = default;
};

template <>
struct std::hash<::named_hash>
{
    constexpr std::size_t operator()(const ::named_hash& h) const noexcept {
        return h.value;
    }
};

struct ast_expands;

struct ast_expands_copy : public load_ast
{
public:
    std::vector<load_ast*> expandable_values;

    template <typename T>
    T* get_expansion_part()
    {
        for (auto x : expandable_values)
        {
            T* child = dynamic_cast<T*>(x);

            if (child) return child;
        }

        return nullptr; // TODO error, as it shouldn't get to here?
    }

    ast_expands_copy(std::string var_name, ast_expands* copied);

    ast_expands_copy(ast_expands_copy* copied) : load_ast(copied->name)
    {
        for (auto x : copied->expandable_values)
        {
            if (x)
            {
                expandable_values.push_back(x->copy());
            }

            else expandable_values.push_back(nullptr);
        }
    }

    void use_editor() override
    {
        if (ImGui::TreeNodeEx(this, load_ast_flags, "%s", name.c_str()))
        {
            for (auto x : expandable_values)
            {
                if (x) x->use_editor();

                else add_err_block();
            }

            ImGui::TreePop();
        }
    }

    load_ast* copy() override
    {
        return new ast_expands_copy(this);
    }
};

struct ast_expands : public load_ast
{
public:
    std::vector<load_ast*>* expandable_values;

    ast_expands()
    {
        expandable_values = new std::vector<load_ast*>;
    }

    ast_expands(std::string class_name) : load_ast(class_name)
    {
        expandable_values = new std::vector<load_ast*>;
    }

    void add_to_values(load_ast* val)
    {
        expandable_values->push_back(val);
    }

    void use_editor() override // Realistically should only be used for root level expansions
    {
        if (ImGui::TreeNodeEx(this, load_ast_flags, "%s", name.c_str()))
        {
            for (auto x : *expandable_values)
            {
                if (x) x->use_editor();

                else add_err_block();
            }

            ImGui::TreePop();
        }
    }

    load_ast* copy() override // Must return ast_expands_copy*
    {
        return new ast_expands_copy(name, this);
    }
};

ast_expands_copy::ast_expands_copy(std::string var_name, ast_expands* copied) : load_ast(var_name)
{
    for (auto x : *copied->expandable_values)
    {
        if (x)
        {
            expandable_values.push_back(x->copy());
        }

        else expandable_values.push_back(nullptr);
    }
}

static std::unordered_map<named_hash, std::vector<ast_expands**>> process_to_be_linked;

struct ast_process_link : public load_ast
{
public:
    ast_expands* expands_to = nullptr;
    named_hash expansion_info;

    ast_process_link(std::string var_name, named_hash class_name) : load_ast(var_name), expansion_info(class_name)
    {
        process_to_be_linked[expansion_info].push_back(&expands_to);
    }

    load_ast* copy() override
    {
        if (expands_to)
        {
            ast_expands_copy* copied = new ast_expands_copy(name, expands_to);

            return copied;
        }

        else return nullptr;
    }
};

static std::unordered_map<named_hash, std::vector<ast_expands**>> complex_to_be_linked;

struct ast_complex_link : public load_ast
{
public:
    ast_expands* expands_to = nullptr;
    named_hash expansion_info;

    ast_complex_link(std::string var_name, named_hash class_name) : load_ast(var_name), expansion_info(class_name)
    {
        complex_to_be_linked[expansion_info].push_back(&expands_to);
    }

    load_ast* copy() override
    {
        if (expands_to)
        {
            ast_expands_copy* copied = new ast_expands_copy(name, expands_to);

            return copied;
        }

        else return nullptr;
    }
};

template <typename T>
constexpr ImGuiDataType get_data_type()
{
    // TODO sizeof(T)
    if (std::is_signed_v<T>)
    {
        switch (sizeof(T))
        {
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

struct ast_value_i : public load_ast
{
private:
    int v = 0;
public:
    ast_value_i(std::string lhs) : load_ast(lhs) {}

    virtual void use_editor() override
    {
        ImGui::InputInt(name.c_str(), &v);
    }

    load_ast* copy() override
    {
        return new ast_value_i(name);
    }
};
struct ast_value_ui : public load_ast
{
private:
    unsigned int v = 0;
public:
    ast_value_ui(std::string lhs) : load_ast(lhs) {}

    virtual void use_editor() override
    {
        ImGui::InputScalar(name.c_str(), get_data_type<unsigned int>(),&v);
    }

    load_ast* copy() override
    {
        return new ast_value_ui(name);
    }
};
struct ast_value_l : public load_ast
{
private:
    long v = 0;
public:
    ast_value_l(std::string lhs) : load_ast(lhs) {}

    virtual void use_editor() override
    {
        ImGui::InputScalar(name.c_str(), get_data_type<long>(),&v);
    }

    load_ast* copy() override
    {
        return new ast_value_l(name);
    }
};
struct ast_value_ul : public load_ast
{
private:
    unsigned long v = 0;
public:
    ast_value_ul(std::string lhs) : load_ast(lhs) {}

    virtual void use_editor() override
    {
        ImGui::InputScalar(name.c_str(), get_data_type<unsigned long>(),&v);
    }

    load_ast* copy() override
    {
        return new ast_value_ul(name);
    }
};
struct ast_value_ull : public load_ast
{
private:
    unsigned long long v = 0;
public:
    ast_value_ull(std::string lhs) : load_ast(lhs) {}

    virtual void use_editor() override
    {
        ImGui::InputScalar(name.c_str(), get_data_type<unsigned long long>(),&v);
    }

    load_ast* copy() override
    {
        return new ast_value_ull(name);
    }
};
struct ast_value_uli : public load_ast
{
private:
    unsigned long int v = 0;
public:
    ast_value_uli(std::string lhs) : load_ast(lhs) {}

    virtual void use_editor() override
    {
        ImGui::InputScalar(name.c_str(), get_data_type<unsigned long int>(),&v);
    }

    load_ast* copy() override
    {
        return new ast_value_uli(name);
    }
};
struct ast_value_c : public load_ast
{
private:
    char v = 0;
public:
    ast_value_c(std::string lhs) : load_ast(lhs) {}

    virtual void use_editor() override
    {
        ImGui::InputText(name.c_str(), &v, 1);
    }

    load_ast* copy() override
    {
        return new ast_value_c(name);
    }
};
struct ast_value_uc : public load_ast
{
private:
    unsigned char v = 0;
public:
    ast_value_uc(std::string lhs) : load_ast(lhs) {}

    virtual void use_editor() override
    {
        ImGui::InputScalar(name.c_str(), get_data_type<unsigned char>(),&v);
    }

    load_ast* copy() override
    {
        return new ast_value_uc(name);
    }
};
struct ast_value_b : public load_ast
{
private:
    bool v = 0;
public:
    ast_value_b(std::string lhs) : load_ast(lhs) {}

    virtual void use_editor() override
    {
        ImGui::Checkbox(name.c_str(), &v);
    }

    load_ast* copy() override
    {
        return new ast_value_b(name);
    }
};
struct ast_value_d : public load_ast
{
private:
    double v = 0;
public:
    ast_value_d(std::string lhs) : load_ast(lhs) {}

    virtual void use_editor() override
    {
        ImGui::InputDouble(name.c_str(), &v);
    }

    load_ast* copy() override
    {
        return new ast_value_d(name);
    }
};
struct ast_value_ld : public load_ast
{
private:
    long double v = 0;
public:
    ast_value_ld(std::string lhs) : load_ast(lhs) {}

    virtual void use_editor() override
    {
        ImGui::InputScalar(name.c_str(), get_data_type<long double>(),&v);
    }

    load_ast* copy() override
    {
        return new ast_value_ld(name);
    }
};

load_ast* create_ast(std::string lhs, std::string function_name, named_hash template_arg)
{
    hash func_hash(function_name.c_str());
    
    switch (func_hash.value)
    {
    case hash("load_complex").value:
    case hash("load_complex_ptr").value:
        return new ast_complex_link(lhs, template_arg);

    case hash("load_data").value:
        {
            switch (template_arg.value)
            {
            case hash("int").value:
                return new ast_value_i(lhs);
            case hash("unsigned int").value:
                return new ast_value_ui(lhs);
            case hash("long").value:
                return new ast_value_l(lhs);
            case hash("unsigned long").value:
                return new ast_value_ul(lhs);
            case hash("unsigned long int").value:
                return new ast_value_uli(lhs);
            case hash("unsigned long long").value:
                return new ast_value_ull(lhs);
            case hash("char").value:
                return new ast_value_c(lhs);
            case hash("unsigned char").value:
                return new ast_value_uc(lhs);
            case hash("bool").value:
                return new ast_value_b(lhs);
            case hash("double").value:
                return new ast_value_d(lhs);
            case hash("long double").value:
                return new ast_value_ld(lhs);
            default:
                return nullptr;
            }
        }

    case hash("load_process").value:
        return new ast_process_link(lhs, template_arg);

    case hash("load_enum").value:
        // TODO ???
        return new ast_value_ull(lhs);

    default:
        return nullptr;
    }
}