
// TODO make all packaged classes have a base ast_* Maybe add a tool to autogen these

#pragma once

#include "ast/ast.hpp"
#include "ast/ast_load.hpp"
#include "ast/ast_stuff.hpp"
#include "ast/print.hpp"

#include ".hpp/pos.hpp"
#include "imgui.h"

#include <string>

struct ast_process : public ast_expands
{
public:
    std::string* obj_name;

    ast_process() : ast_expands("Process")
    {
        
    }
};


struct ast_object : public ast_expands
{
public:
    pos* position;

    ast_object() : ast_expands("Process")
    {
        expandable_values->push_back(new ast_complex_link("position", named_hash("pos")));

        expandable_values->push_back(new ast_complex_link("angle", named_hash("rad")));
    }
};

struct SDL_Texture;
struct ast_drawobj : public load_ast
{
public:
    SDL_Texture* texture;
};

struct ast_blendobj : public load_ast
{
public:
};

struct ast_collobj : public load_ast
{
public:
};

void init_base_loads()
{
    loadable_processes[hash("Process").value] = new ast_process;

    loadable_processes[hash("Object").value] = new ast_object;
}