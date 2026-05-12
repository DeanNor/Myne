
#pragma once

#include ".hpp/hash.hpp"
#include "ast/ast_search.hpp"
#include "ast/ast_stuff.hpp"
#include "ast/print.hpp"
#include "ast/ts_tool.h"

#include <fstream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <unordered_map>

struct late_class
{
public:
    load_expandable_base* expansion = nullptr;
    
    std::string& str_parent;
    hash& parent_name;

    late_class(class_hint* _what) : str_parent(_what->str_parent), parent_name(_what->parent_name) {}

    load_expandable_base* get_expansion(std::unordered_map<hash_t, load_expandable_base*>& loads, std::unordered_map<hash_t, late_class*>& lates)
    {
        if (expansion) return expansion;
        
        try
        {
            late_class* direct_parent = lates.at(parent_name.value);

            expansion = direct_parent->get_expansion(loads, lates);

            return expansion;
        }

        catch (...)
        {
            std::cout << "Error late linking loadable:\n\t" << str_parent << ' ' << parent_name.value << '\n'; // No load found

            return nullptr;
        }

        try
        {
            expansion = loads.at(parent_name.value);
        
            return expansion;
        }

        catch (...)
        {
            std::cout << "Error late linking loadable:\n\t" << str_parent << ' ' << parent_name.value << '\n'; // No load found

            return nullptr;
        }
    }
};

inline void add_to_map(class_hint* hint, std::unordered_map<hash_t, load_expandable_base*>& map, std::unordered_map<hash_t, late_class*>& late)
{
    try
    {
        load_expandable* expansion = found_loads.at(hint->class_name.value);

        expansion->is_class = hint->is_class;
        map.emplace(hint->class_name.value,expansion);
    }

    catch (...) // For classes that do not explicitly define a load function, but have a parent who (SHOULD) implicitly gives it to them
    {
        if (hint->inherits)
        {
            late.emplace(hint->class_name.value,new late_class(hint));
        }

        else
        {
            // TODO queue error here and below
            std::cout << "Error linking loadable:\n\t" << hint->str_name << ' ' << hint->class_name.value << '\n'; // No load found
        }
    }
}

// Throws if not found in list of objects
inline load_expandable_base* get_expandable_process(hash_t type)
{
    if (loadable_processes.contains(type)) return loadable_processes.at(type);

    else if (base_loads_process.contains(type)) return base_loads_process.at(type);

    else throw std::logic_error(std::string("Bad Type") + std::to_string(type));
}

// Throws if not found in list of objects
inline load_expandable_base* get_expandable_complex(hash_t type)
{
    try
    {
        return loadable_complexes.at(type);
    }

    catch (...)
    {
        return base_loads_complex.at(type);
    }
}

// Links the loadable_* and finds the stuff to be linked
inline void link_with_classes()
{
    std::unordered_map<hash_t, late_class*> late_linkers_process;
    std::unordered_map<hash_t, late_class*> late_linkers_complex;

    for (auto x : found_loaders)
    {
        if (x->is_class == true)
        {
            add_to_map(x, loadable_processes, late_linkers_process);
        }

        else
        {
            add_to_map(x, loadable_complexes, late_linkers_complex);
        }
    }

    for (auto& x : late_linkers_process)
    {
        load_expandable_base* late_expansion = x.second->get_expansion(loadable_processes, late_linkers_process);

        if (late_expansion)
        {
            loadable_processes.emplace(x.first, late_expansion);
        }

        else 
        {
            std::cout << "Error late linking loadable process with parent: \n\t" << x.second->str_parent << ' ' << x.second->parent_name.value << '\n';
        }
    }

    for (auto& x : late_linkers_complex)
    {
        load_expandable_base* late_expansion = x.second->get_expansion(loadable_complexes, late_linkers_complex);

        if (late_expansion)
        {
            loadable_complexes.emplace(x.first, late_expansion);
        }

        else 
        {
            std::cout << "Error late linking loadable complex with parent: \n\t" << x.second->str_parent << ' ' << x.second->parent_name.value << '\n';
        }
    }

    for (auto x : parent_to_be_linked)
    {
        try
        {
            if (x.second->is_class)
            {
                x.second->parent_expansion = get_expandable_process(x.first);
            }

            else
            {
                x.second->parent_expansion = get_expandable_complex(x.first);
            }
        }

        catch (...)
        {
            std::cout << "ERROR LINKING PARENT IN CLASS " << x.second->var_name << " with type " << x.first << std::endl;
        }
    }
}

void add_class(TSNode process, TSNode class_ast, const char* file, bool is_class);

inline void search_definition(std::string file_loc)
{
    std::ifstream ifile(file_loc);

    if (!ifile) return; // TODO cache error;

    std::string file_str = std::string(std::istreambuf_iterator<char>(ifile),std::istreambuf_iterator<char>());
    const char* file_content = file_str.c_str();

    TSParser *parser = ts_parser_new();
    ts_parser_set_language(parser, tree_sitter_cpp());

    TSTree *tree = ts_parser_parse_string(parser, NULL, file_content, file_str.size());
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
                                        hash id_name = hash::hash_for(ts_node_start_byte(identifier) + file_content, ts_node_end_byte(identifier) - ts_node_start_byte(identifier));

                                        switch (id_name.value)
                                        {
                                        case hash("ASSIGN_CONSTRUCTOR").value:
                                        case hash("ASSIGN_CONSTRUCTOR_OVERRIDE").value:
                                            add_class(parameter_declaration, class_specifier, file_content, true);
                                            break;

                                        case hash("ASSIGN_VAR_CONSTRUCTOR").value:
                                        case hash("ASSIGN_VIR_VAR_CONSTRUCTOR").value:
                                        case hash("ASSIGN_VIR_VAR_CONSTRUCTOR_OVERRIDE").value:
                                            add_class(parameter_declaration, class_specifier, file_content, false);
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

inline bool get_base_class(TSNode class_ast, const char* file, std::string& ret_str)
{
    TSNode base_class_clause, type_identifier;
    if (search_for_node(class_ast, refs->base_class_clause, base_class_clause))
    {
        if (search_for_node(base_class_clause, refs->type_identifier, type_identifier))
        {
            ret_str = std::string(file + ts_node_start_byte(type_identifier), file + ts_node_end_byte(type_identifier));

            return true;
        }
    }

    return false;
}

inline void add_class(TSNode process, TSNode class_ast, const char* file, bool is_class)
{
    std::string name(file + ts_node_start_byte(process), file + ts_node_end_byte(process));

    std::string base_class;
    if (get_base_class(class_ast, file, base_class))
    {
        found_loaders.push_back(new class_hint(is_class, name, base_class.c_str()));
    }

    else
    {
        if (is_class) std::cout << "CLASS: " << name << " does not inherit from anything???\n";

        found_loaders.push_back(new class_hint(is_class, name));
    }
}