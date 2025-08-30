
#pragma once

#include ".hpp/hash.hpp"
#include "ast_stuff.hpp"
#include "tree_sitter/api.h"
#include <cstring>
#include <stdexcept>
#include <string>
#include <fstream>
#include "ts_tool.h"
#include "print.hpp"

extern std::string file;
extern ref_storer* refs;
extern std::unordered_map<hash_t, ast_expands*> found_loads;

load_ast* get_load_type_from_rhs(TSNode expr)
{
    if (ts_node_named_child_count(expr) <= 0) return nullptr;

    const char* expr_type = ts_node_type(expr);

    if (expr_type == refs->call_expr)
    {
        TSNode body = ts_node_named_child(expr, 0);

        if (ts_node_named_child_count(body) > 0)
        {
            TSNode lhs = ts_node_named_child(body, 0);

            TSNode function = ts_node_named_child(body, 1);
            
            TSNode identifier = ts_node_named_child(function, 0);
            TSNode template_list = ts_node_named_child(function, 1);

            std::string func_name(file.c_str() + ts_node_start_byte(identifier), file.c_str() + ts_node_end_byte(identifier));
            named_hash tmplt_str;

            for (uint32_t x = 0; x < ts_node_named_child_count(template_list); ++x)
            {
                TSNode tmplt = ts_node_named_child(template_list, x);
                
                std::string tmplt_name;
                tmplt_str = hash::hash_for_str(file.c_str() + ts_node_start_byte(tmplt), ts_node_end_byte(tmplt) - ts_node_start_byte(tmplt), tmplt_name);

                tmplt_str.name = tmplt_name;
            }

            return create_ast(std::string(file.c_str() + ts_node_start_byte(lhs), file.c_str() + ts_node_end_byte(lhs)), func_name, tmplt_str);
        }

        else // If conversion or called function like int(load_smt) or does(load_smt)
        {
            return get_load_type_from_rhs(ts_node_named_child(expr, ts_node_named_child_count(expr) - 1));
        }
    }

    else
    {
        return get_load_type_from_rhs(ts_node_named_child(expr, ts_node_named_child_count(expr) - 1));
    }
}

std::vector<std::pair<named_hash, ast_expands*>> unknown_to_be_linked;

void search_load_data(TSNode body, ast_expands* ast_object)
{
    uint32_t children = ts_node_named_child_count(body);

    for (uint32_t x = 0; x < children; ++x)
    {
        TSNode expr = ts_node_named_child(body, x);
        if (ts_node_type(expr) == refs->expr_stmt)
        {
            expr = ts_node_named_child(expr, 0);
            const char* expr_type = ts_node_type(expr);

            if (expr_type == refs->call_expr)
            {
                TSNode qualified_identifier, namespace_identifier, identifier;
                if (search_for_node(expr, refs->qualified_identifier, qualified_identifier))
                {
                    if (search_for_node(qualified_identifier, refs->namespace_identifier, namespace_identifier) && search_for_node(qualified_identifier, refs->identifier, identifier))
                    {
                        hash func_name = hash::hash_for(file.c_str() + ts_node_start_byte(identifier), ts_node_end_byte(identifier) - ts_node_start_byte(identifier));
                        if (func_name == hash("load"))
                        {
                            std::string class_name_str;
                            named_hash class_name = hash::hash_for_str(file.c_str() + ts_node_start_byte(namespace_identifier), ts_node_end_byte(namespace_identifier) - ts_node_start_byte(namespace_identifier), class_name_str);
                            class_name.name = class_name_str;

                            unknown_to_be_linked.emplace_back(class_name, ast_object);
                        }
                    }
                }
            }
            
            else if (expr_type == refs->assignment_expression)
            {
                auto a = get_load_type_from_rhs(ts_node_named_child(expr, ts_node_named_child_count(expr) - 1));

                if (a) ast_object->add_to_values(a);
            }
        }
    }
}


void search_load_content(TSNode content);

void search_load_class_content(TSNode content, TSNode class_ast);

void search_load_out(TSNode function_def);

void search_load_in(TSNode function_def, TSNode class_ast);

void search_load(std::string file_loc) // TODO external versions
{
    std::ifstream ifile(file_loc);

    if (!ifile)
    {
        std::cout << "Bad File: " << file_loc << std::endl;

        return;
    }; // TODO cache error

    file = std::string(std::istreambuf_iterator<char>(ifile),std::istreambuf_iterator<char>()); // TODO multi thread the below task and have a daemeon to check for file changes
    
    TSParser *parser = ts_parser_new();
    ts_parser_set_language(parser, tree_sitter_cpp());

    TSTree *tree = ts_parser_parse_string(parser, NULL, file.c_str(), file.size());
    TSNode root = ts_tree_root_node(tree);

    search_load_content(root);

    ts_tree_delete(tree);
    ts_parser_delete(parser);
}

inline void search_class(TSNode class_ast)
{
    TSNode type_identifier, field_declaration_list;
    if (search_for_node(class_ast, refs->type_identifier, type_identifier) && search_for_node(class_ast, refs->field_declaration_list, field_declaration_list))
    {
        search_load_class_content(field_declaration_list, type_identifier);
    }
}

void search_load_content(TSNode content)
{
    uint32_t child_count = ts_node_named_child_count(content);
    for (uint32_t i = 0; i < child_count; i++)
    {
        TSNode node = ts_node_named_child(content, i);

        const char* node_type = ts_node_type(node);

        if (node_type == refs->function_def)
        {
            search_load_out(node);
        }

        else if (node_type == refs->class_specifier || node_type == refs->struct_specifier)
        {
            search_class(node);
        }
    }
}

void search_load_class_content(TSNode content, TSNode class_ast)
{
    uint32_t child_count = ts_node_named_child_count(content);
    for (uint32_t i = 0; i < child_count; i++)
    {
        TSNode node = ts_node_named_child(content, i);

        const char* node_type = ts_node_type(node);

        if (node_type == refs->function_def)
        {
            search_load_in(node, class_ast);
        }

        else if (node_type == refs->class_specifier || node_type == refs->struct_specifier)
        {
            search_class(node);
        }
    }
}

void create_load_ast(TSNode identifier, TSNode namespace_identifier, TSNode compound_statement)
{
    hash hsh = hash::hash_for(file.c_str() + ts_node_start_byte(identifier), ts_node_end_byte(identifier) - ts_node_start_byte(identifier));
    if (hsh == hash("load"))
    {
        std::string name;
        hash class_name = hash::hash_for_str(file.c_str() + ts_node_start_byte(namespace_identifier), ts_node_end_byte(namespace_identifier) - ts_node_start_byte(namespace_identifier), name);

        ast_expands* expansion = new ast_expands(name);
        search_load_data(compound_statement, expansion);

        found_loads[class_name.value] = expansion;
    }
}

void search_load_out(TSNode function_def)
{
    TSNode function_declarator;
    if (search_for_node(function_def, refs->function_declarator, function_declarator))
    {
        TSNode qualified_identifier, namespace_identifier, identifier, compound_statement;
        if (search_for_node(function_def, refs->compound_statement, compound_statement))
        {
            if(search_for_node(function_declarator, refs->qualified_identifier, qualified_identifier))
            {
                if (search_for_node(qualified_identifier, refs->identifier, identifier))
                {
                    if (search_for_node(qualified_identifier, refs->namespace_identifier, namespace_identifier))
                    {
                        create_load_ast(identifier, namespace_identifier, compound_statement);
                    }

                    else 
                    {
                        hash hsh = hash::hash_for(file.c_str() + ts_node_start_byte(identifier), ts_node_end_byte(identifier) - ts_node_start_byte(identifier));
                        if (hsh == hash("OVERRIDE_LOAD"))
                        {
                            // TODO
                            throw std::runtime_error("BAD!");
                        }
                    }
                }
            }
        }
    }
}

void search_load_in(TSNode function_def, TSNode class_ast)
{
    TSNode function_declarator, field_identifier, compound_statement;
    if (search_for_node(function_def, refs->function_declarator, function_declarator) && search_for_node(function_def, refs->compound_statement, compound_statement))
    {
        if (search_for_node(function_declarator, refs->field_identifier, field_identifier))
        {
            create_load_ast(field_identifier, class_ast, compound_statement);
        }
    }
}