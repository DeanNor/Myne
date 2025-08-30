
#pragma once

#include "tree_sitter/api.h"
#include <iostream>

extern std::string file;

void print_node(TSNode root)
{

    std::cout << ts_node_type(root) << '\n';

    std::cout << std::string(file.c_str() + ts_node_start_byte(root), file.c_str() + ts_node_end_byte(root)) << '\n';

    uint32_t child_count = ts_node_named_child_count(root);
    for (uint32_t i = 0; i < child_count; i++)
    {
        TSNode node = ts_node_named_child(root, i);

        std::cout << "Iter: " << i  << '\n';

        print_node(node);
    }

    std::cout << std::endl;
}

void print_node_base(TSNode root)
{

    std::cout << ts_node_type(root) << '\n';
    
    uint32_t child_count = ts_node_named_child_count(root);
    for (uint32_t i = 0; i < child_count; i++)
    {
        TSNode node = ts_node_named_child(root, i);

        std::cout << "Iter: " << i  << '\n';

        std::cout << ts_node_type(node) << '\n';
    }

    std::cout << std::endl;
}

inline bool search_for_node(TSNode parent, const char* type, TSNode& ret) // TODO replace all ts_node_named_child calls with this
{
    for (uint32_t x = 0; x < ts_node_named_child_count(parent); ++x)
    {
        TSNode child = ts_node_named_child(parent, x);

        if (ts_node_type(child) == type)
        {
            ret = child;
            return true;
        }

        if (ts_node_has_error(child))
        {
            if (search_for_node(child, type, ret)) return true;
        }
    }

    return false;
}