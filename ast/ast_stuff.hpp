
#pragma once

#include "./.hpp/hash.hpp"

#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

struct load_ast
{
public:
    std::string name;

    virtual void use_editor() = 0;

    virtual ~load_ast()
    {
        
    }

    load_ast(std::string variable_name) : name(variable_name)
    {

    }

    load_ast() = default;
};

struct ast_expands : public load_ast
{
private:
    std::vector<load_ast*>* expandable_values = nullptr;

public:
    ast_expands(std::string class_name) : load_ast(class_name)
    {
        // TODO attach to a lookup table
        expandable_values = new std::vector<load_ast*>;
    }

    void add_to_values(load_ast* val)
    {
        expandable_values->push_back(val);
    }

    void use_editor() override
    {
        std::cout << "-----EXPANDING\n";

        for (auto x : *expandable_values)
        {
            x->use_editor();
        }
        std::cout << "-----DONE\n\n";
    }
};

struct named_hash : public hash
{
public:
    std::string name;

    named_hash(std::string to_hash) : hash(to_hash.c_str()), name(to_hash)
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

static std::unordered_map<named_hash, std::vector<ast_expands**>> process_to_be_linked;

struct ast_process_link : public load_ast
{
    ast_expands* expands_to = nullptr;
    named_hash expansion_info;

    ast_process_link(std::string var_name, named_hash class_name) : load_ast(var_name), expansion_info(class_name)
    {
        process_to_be_linked[expansion_info].push_back(&expands_to);
    }

    void use_editor() override
    {
        if (expands_to) expands_to->use_editor();
        else std::cout << "ERR UNLINKED EXPANSION " << expansion_info.name << '\n';
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

    void use_editor() override
    {
        if (expands_to) expands_to->use_editor();
        else std::cout << "ERR UNLINKED EXPANSION " << expansion_info.name << ' ' << expansion_info.value << '\n';
    }
};

struct ast_value_i : public load_ast
{
private:
    int v = 0;
public:
    ast_value_i(std::string lhs) : load_ast(lhs) {}

    virtual void use_editor() override
    {

        std::cout << "VAL" << std::endl;
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

        std::cout << "VAL" << std::endl;
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

        std::cout << "VAL" << std::endl;
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

        std::cout << "VAL" << std::endl;
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

        std::cout << "VAL" << std::endl;
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

        std::cout << "VAL" << std::endl;
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

        std::cout << "VAL" << std::endl;
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
        std::cout << "VAL" << std::endl;
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

        std::cout << "VAL" << std::endl;
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

        std::cout << "VAL" << std::endl;
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

        std::cout << "VAL" << std::endl;
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