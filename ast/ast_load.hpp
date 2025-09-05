
#pragma once

#include ".hpp/hash.hpp"
#include "ast/ast_search.hpp"
#include "ast/ast_stuff.hpp"
#include "ast/print.hpp"
#include "ast/ts_tool.h"
#include "imgui.h"

#include <mutex>
#include <fstream>

struct named_class
{
public:
    bool is_class;

    std::string name;
    ast_expands* ast_expansion;

    bool inherits = false;
    hash base_class;

    named_class(bool _is_class, std::string _name) : is_class(_is_class), name(_name)
    {

    }


    named_class(bool _is_class, std::string _name, hash _base_class) : is_class(_is_class), name(_name), inherits(true), base_class(_base_class)
    {

    }
};

std::mutex loader_lock;
std::vector<named_class*> found_loaders;
std::unordered_map<hash_t, ast_expands*> found_loads;

// TODO list of inheritances to show in editor
static std::unordered_map<hash_t, ast_expands*> loadable_processes;
static std::unordered_map<hash_t, ast_expands*> loadable_complexes;

extern std::vector<std::pair<named_hash, ast_expands*>> unknown_to_be_linked;

ref_storer* refs;

// Links the loadable_* and finds the stuff to be linked
void link_with_classes()
{
    loader_lock.lock();

    std::vector<std::pair<hash_t, named_class*>> late_linkers_process;
    std::vector<std::pair<hash_t, named_class*>> late_linkers_complex;

    for (auto x : found_loaders)
    {
        named_hash hash_name = x->name;
        if (x->is_class == true)
        {
            x->ast_expansion = found_loads[hash_name.value];

            if (x->ast_expansion)
            {
                loadable_processes[hash_name.value] = x->ast_expansion;
            }

            else
            {
                if (x->inherits)
                {
                    late_linkers_process.emplace_back(hash_name.value, x);
                }

                else
                {
                    // TODO queue error here and below
                    std::cout << "Error linking loadable Processes:\n\t" << x->name << ' ' << hash_name.value << '\n'; // No load found
                }
            }
        }

        else
        {
            x->ast_expansion = found_loads[hash_name.value];

            if (x->ast_expansion)
            {
                loadable_complexes[hash_name.value] = x->ast_expansion;
            }

            else
            {
                if (x->inherits)
                {
                    late_linkers_complex.emplace_back(hash_name.value, x);
                }

                else
                {
                    std::cout << "Error linking loadable Complexes:\n\t" << x->name << ' ' << hash_name.value << '\n'; // No load found
                }
            }
        }
    }

    for (auto x : late_linkers_process)
    {
        auto parent = loadable_processes[x.second->base_class.value];
        if (parent)
        {
            x.second->ast_expansion = parent;

            loadable_processes[x.first] = x.second->ast_expansion;
        }

        else 
        {
            std::cout << "Error late linking loadable Processes: \n\t" << x.second->name << ' ' << x.first << ' ' << x.second->base_class.value << '\n';
        }
    }

    for (auto x : late_linkers_complex)
    {
        auto parent = loadable_complexes[x.second->base_class.value];
        if (parent)
        {
            x.second->ast_expansion = parent;

            loadable_complexes[x.first] = x.second->ast_expansion;
        }

        else 
        {
            std::cout << "Error late linking loadable Complexes: \n\t" << x.second->name << ' ' << x.first << ' ' << x.second->base_class.value << '\n';
        }
    }

    for (auto x : process_to_be_linked)
    {
        auto expansion = loadable_processes[x.first.value];

        if (expansion)
        {
            for (auto y : x.second)
            {
               (*y) = dynamic_cast<ast_expands*>(expansion);
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
               (*y) = dynamic_cast<ast_expands*>(expansion);
            }
        }

        else std::cout << "ERR COMPLEX_LINK " << x.first.name << '\n';
    }

    for (auto x : unknown_to_be_linked)
    {
        auto process_expansion = loadable_processes[x.first.value];
        if (process_expansion)
        {
            x.second->add_to_values(process_expansion);
        }

        else
        {
            auto complex_expansion = loadable_complexes[x.first.value];
            if (complex_expansion)
            {
                x.second->add_to_values(complex_expansion);
            }

            else
            {
                std::cout << "ERR UNKNOWN_LINK " << x.first.name << '\n';
            }
        }
    }

    loader_lock.unlock();
}

void add_process_class(TSNode process, TSNode class_ast, const char* file);
void add_complex_class(TSNode complex, TSNode class_ast, const char* file);

void search_definition(std::string file_loc)
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
                                            add_process_class(parameter_declaration, class_specifier, file_content);
                                            break;

                                        case hash("ASSIGN_VAR_CONSTRUCTOR").value:
                                        case hash("ASSIGN_VIR_VAR_CONSTRUCTOR").value:
                                        case hash("ASSIGN_VIR_VAR_CONSTRUCTOR_OVERRIDE").value:
                                            add_complex_class(parameter_declaration, class_specifier, file_content);
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

void add_process_class(TSNode process, TSNode class_ast, const char* file)
{
    std::string name(file + ts_node_start_byte(process), file + ts_node_end_byte(process));

    std::string base_class;
    if (get_base_class(class_ast, file, base_class))
    {
        loader_lock.lock();
        found_loaders.push_back(new named_class(true, name, base_class.c_str()));
        loader_lock.unlock();
    }

    else
    {
        std::cout << "CLASS: " << name << " does not inherit from anything???\n";
        loader_lock.lock();
        found_loaders.push_back(new named_class(true, name));
        loader_lock.unlock();
    }
}

void add_complex_class(TSNode complex, TSNode class_ast, const char* file)
{
    std::string name(file + ts_node_start_byte(complex), file + ts_node_end_byte(complex));

    std::string base_class;
    if (get_base_class(class_ast, file, base_class))
    {
        loader_lock.lock();
        found_loaders.push_back(new named_class(false, name, base_class.c_str()));
        loader_lock.unlock();
    }

    else 
    {
        loader_lock.lock();
        found_loaders.push_back(new named_class(false, name));
        loader_lock.unlock();
    }
}