
#pragma once

#include ".hpp/hash.hpp"
#include "ast/ast_search.hpp"
#include "ast/ast_stuff.hpp"
#include "ast/print.hpp"
#include "ast/ts_tool.h"

#include <mutex>
#include <fstream>

struct named_class
{
public:
    bool is_class;

    std::string name;
    ast_expands* ast_expansion;

    named_class(bool _is_class, std::string _name) : is_class(_is_class), name(_name)
    {

    }
};

std::vector<named_class*> found_loaders;
std::unordered_map<hash_t, ast_expands*> found_loads;

// TODO list of inheritances to show in editor
std::unordered_map<hash_t, named_class*> loadable_processes;
std::unordered_map<hash_t, named_class*> loadable_complexes;

ref_storer* refs;
std::string file;

// Links the loadable_* and finds the stuff to be linked
void link_with_classes()
{
    for (auto x : found_loaders)
    {
        named_hash hash_name = x->name;
        if (x->is_class == true)
        {
            x->ast_expansion = found_loads[hash_name.value];

            if (x->ast_expansion)
            {
                loadable_processes[hash_name.value] = x;
            }

            else
            {
                // TODO queue error here and below
                std::cout << "Error linking loadable Processes:\n\t" << x->name << ' ' << hash_name.value << '\n';
            }
        }

        else
        {
            x->ast_expansion = found_loads[hash_name.value];

            if (x->ast_expansion)
            {
                loadable_complexes[hash_name.value] = x;
            }

            else
            {
                std::cout << "Error linking loadable Complexes:\n\t" << x->name << ' ' << hash_name.value << '\n';
            }
        }
    }

    for (auto x : process_to_be_linked)
    {
        auto expansion = loadable_processes[x.first.value];

        if (expansion)
        {
            for (auto y : x.second)
            {
               (*y) = expansion->ast_expansion;
            }
        }

        else std::cout << "ERR PROCESS_LINK " << x.first.name << '\n';
    }

    for (auto x : complex_to_be_linked)
    {
        auto expansion = loadable_complexes[x.first.value];
        
        if (expansion)
        {
            for (auto y : x.second)
            {
               (*y) = expansion->ast_expansion;
            }
        }

        else std::cout << "ERR COMPLEX_LINK " << x.first.name << '\n';
    }

    for (auto x : unknown_to_be_linked)
    {
        auto process_expansion = loadable_processes[x.first.value];
        if (process_expansion)
        {
            x.second->add_to_values(process_expansion->ast_expansion);
        }

        else
        {
            auto complex_expansion = loadable_complexes[x.first.value];
            if (complex_expansion)
            {
                x.second->add_to_values(complex_expansion->ast_expansion);
            }

            else
            {
                std::cout << "ERR UNKNOWN_LINK " << x.first.name << '\n';
            }
        }
    }
}

void add_process_class(TSNode process);
void add_complex_class(TSNode complex);

void search_definition(std::string file_loc)
{
    std::ifstream ifile(file_loc);

    if (!ifile) return; // TODO cache error;

    file = std::string(std::istreambuf_iterator<char>(ifile),std::istreambuf_iterator<char>());

    TSParser *parser = ts_parser_new();
    ts_parser_set_language(parser, tree_sitter_cpp());

    TSTree *tree = ts_parser_parse_string(parser, NULL, file.c_str(), file.size());
    TSNode root = ts_tree_root_node(tree);

    for(uint32_t x = 0; x < ts_node_named_child_count(root); ++x)
    {
        TSNode class_specifier = ts_node_named_child(root, x);
        const char* node_type = ts_node_type(class_specifier);
        if (node_type == refs->class_specifier || node_type == refs->struct_specifier) // TODO external stuff
        {
            TSNode field_declaration_list;
            if(search_for_node(class_specifier, refs->field_declaration_list, field_declaration_list))
            {
                for (uint32_t y = 0; y < ts_node_named_child_count(field_declaration_list); ++y)
                {
                    TSNode declaration = ts_node_named_child(field_declaration_list, y);

                    if (ts_node_type(declaration) == refs->declaration)
                    {
                        TSNode function_declarator;
                        
                        if (search_for_node(declaration, refs->function_declarator, function_declarator))
                        {
                            TSNode identifier;
                            if (search_for_node(function_declarator, refs->identifier, identifier))
                            {
                                TSNode parameter_list;

                                if (search_for_node(function_declarator, refs->parameter_list,parameter_list))
                                {
                                    TSNode parameter_declaration;
                                    if (search_for_node(parameter_list, refs->parameter_declaration, parameter_declaration))
                                    {
                                        hash id_name = hash::hash_for(ts_node_start_byte(identifier) + file.c_str(), ts_node_end_byte(identifier) - ts_node_start_byte(identifier));

                                        switch (id_name.value)
                                        {
                                        case hash("ASSIGN_CONSTRUCTOR").value:
                                            add_process_class(parameter_declaration);
                                            break;

                                        case hash("ASSIGN_VAR_CONSTRUCTOR").value:
                                        case hash("ASSIGN_VIR_VAR_CONSTRUCTOR").value:
                                        case hash("ASSIGN_VIR_VAR_CONSTRUCTOR_OVERRIDE").value:
                                            add_complex_class(parameter_declaration);
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void add_process_class(TSNode process)
{
    std::string name(file.c_str() + ts_node_start_byte(process), file.c_str() + ts_node_end_byte(process));

    found_loaders.push_back(new named_class(true, name));
}

void add_complex_class(TSNode complex)
{
    std::string name(file.c_str() + ts_node_start_byte(complex), file.c_str() + ts_node_end_byte(complex));

    found_loaders.push_back(new named_class(false, name));
}