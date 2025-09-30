
#pragma once

#include ".hpp/hash.hpp"
#include "ast/ast_stuff.hpp"
#include "tree_sitter/api.h"
#include <cstring>
#include <stdexcept>
#include <string>
#include <fstream>
#include "ts_tool.h"
#include "print.hpp"

extern ref_storer* refs;
extern std::unordered_map<hash_t, ast_expands*> found_loads;
extern std::mutex loader_lock;

load_ast* get_load_type(TSNode expr, const char* file)
{
    TSNode identifier, call_expression;
    if (search_for_node(expr, refs->identifier, identifier) && search_for_node(expr, refs->call_expr, call_expression))
    {
        std::string lhs = std::string(file + ts_node_start_byte(identifier), file + ts_node_end_byte(identifier));

        TSNode field_expression;
        if (search_for_node(call_expression, refs->field_expression, field_expression))
        {
            TSNode template_method;
            if (search_for_node(field_expression, refs->template_method,template_method))
            {
                TSNode field_identifier, template_argument_list;
                if (search_for_node(template_method, refs->field_identifier, field_identifier) && search_for_node(template_method, refs->template_argument_list, template_argument_list))
                {
                    TSNode type_descriptor;
                    if (search_for_node(template_argument_list, refs->type_descriptor, type_descriptor))
                    {
                        std::string func_name = std::string(file + ts_node_start_byte(field_identifier), file + ts_node_end_byte(field_identifier));
                        std::string tmplt_str = std::string(file + ts_node_start_byte(type_descriptor), file + ts_node_end_byte(type_descriptor));

                        return create_ast(lhs, func_name, tmplt_str.data());
                    }
                }
            }
        }
    }

    return nullptr;
}

std::vector<std::pair<named_hash, ast_expands*>> unknown_to_be_linked;

void search_load_data(TSNode body, ast_expands* ast_object, const char* file)
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
                        hash func_name = hash::hash_for(file + ts_node_start_byte(identifier), ts_node_end_byte(identifier) - ts_node_start_byte(identifier));
                        if (func_name == hash("load"))
                        {
                            std::string class_name_str;
                            named_hash class_name = hash::hash_for_str(file + ts_node_start_byte(namespace_identifier), ts_node_end_byte(namespace_identifier) - ts_node_start_byte(namespace_identifier), class_name_str);
                            class_name.name = class_name_str;

                            loader_lock.lock();
                            unknown_to_be_linked.emplace_back(class_name, ast_object);
                            loader_lock.unlock();
                        }
                    }
                }
            }
            
            else if (expr_type == refs->assignment_expression)
            {
                auto a = get_load_type(expr, file);

                if (a) ast_object->add_to_values(a);
            }
        }
    }
}


void search_load_content(TSNode content, const char* file);

void search_load_class_content(TSNode content, TSNode class_ast, const char* file);

void search_load_out(TSNode function_def, const char* file);

void search_load_in(TSNode function_def, TSNode class_ast, const char* file);

void search_load(std::string file_loc) // TODO external versions
{
    std::ifstream ifile(file_loc);

    if (!ifile)
    {
        std::cout << "Bad File: " << file_loc << std::endl;

        return;
    }; // TODO cache error

    std::string file = std::string(std::istreambuf_iterator<char>(ifile),std::istreambuf_iterator<char>()); // TODO multi thread the below task and have a daemeon to check for file changes
    
    TSParser *parser = ts_parser_new();
    ts_parser_set_language(parser, tree_sitter_cpp());

    TSTree *tree = ts_parser_parse_string(parser, NULL, file.c_str(), file.size());
    TSNode root = ts_tree_root_node(tree);

    search_load_content(root, file.c_str());

    ts_tree_delete(tree);
    ts_parser_delete(parser);
}

inline void search_class(TSNode class_ast, const char* file)
{
    TSNode type_identifier, field_declaration_list;
    if (search_for_node(class_ast, refs->type_identifier, type_identifier) && search_for_node(class_ast, refs->field_declaration_list, field_declaration_list))
    {
        search_load_class_content(field_declaration_list, type_identifier, file);
    }
}

void search_load_content(TSNode content, const char* file)
{
    uint32_t child_count = ts_node_named_child_count(content);
    for (uint32_t i = 0; i < child_count; i++)
    {
        TSNode node = ts_node_named_child(content, i);

        const char* node_type = ts_node_type(node);

        if (node_type == refs->function_def)
        {
            search_load_out(node, file);
        }

        else if (node_type == refs->class_specifier || node_type == refs->struct_specifier)
        {
            search_class(node, file);
        }
    }
}

void search_load_class_content(TSNode content, TSNode class_ast, const char* file)
{
    uint32_t child_count = ts_node_named_child_count(content);
    for (uint32_t i = 0; i < child_count; i++)
    {
        TSNode node = ts_node_named_child(content, i);

        const char* node_type = ts_node_type(node);

        if (node_type == refs->function_def)
        {
            search_load_in(node, class_ast, file);
        }

        else if (node_type == refs->class_specifier || node_type == refs->struct_specifier)
        {
            search_class(node, file);
        }
    }
}

void create_load_ast(TSNode identifier, TSNode namespace_identifier, TSNode compound_statement, const char* file)
{
    hash hsh = hash::hash_for(file + ts_node_start_byte(identifier), ts_node_end_byte(identifier) - ts_node_start_byte(identifier));
    if (hsh == hash("load"))
    {
        std::string name;
        hash class_name = hash::hash_for_str(file + ts_node_start_byte(namespace_identifier), ts_node_end_byte(namespace_identifier) - ts_node_start_byte(namespace_identifier), name);

        ast_expands* expansion = new ast_expands(name);
        search_load_data(compound_statement, expansion, file);

        found_loads[class_name.value] = expansion;
    }
}

void search_load_out(TSNode function_def, const char* file)
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
                        create_load_ast(identifier, namespace_identifier, compound_statement, file);
                    }

                    else 
                    {
                        hash hsh = hash::hash_for(file + ts_node_start_byte(identifier), ts_node_end_byte(identifier) - ts_node_start_byte(identifier));
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

void search_load_in(TSNode function_def, TSNode class_ast, const char* file)
{
    TSNode function_declarator, field_identifier, compound_statement;
    if (search_for_node(function_def, refs->function_declarator, function_declarator) && search_for_node(function_def, refs->compound_statement, compound_statement))
    {
        if (search_for_node(function_declarator, refs->field_identifier, field_identifier))
        {
            create_load_ast(field_identifier, class_ast, compound_statement, file);
        }
    }
}