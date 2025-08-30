
#pragma once

#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <iostream>

#include "ast/ast_search.hpp"
#include "ast/ast_stuff.hpp"
#include "ast/ts_tool.h"

#include "ast/ast_load.hpp"

// TODO make all the error windows here, and in other ast_* files, present to a buffer to be displayed

std::vector<std::string> header_files;
std::vector<std::string> code_files;

void check_file_changes();

void load_files();

inline void setup_ast()
{
    refs = construct_storer();

    load_files();
}

inline void exit_ast()
{
    free(refs);
}

void load_files()
{
    // TODO load from file
    header_files = {"./.hpp/object.hpp","./.hpp/drawobj.hpp","./.hpp/pos.hpp","./.hpp/rad.hpp"};

    code_files = {"./.cpp/object.cpp","./.cpp/drawobj.cpp","./.cpp/pos.cpp","./.hpp/rad.hpp"};
}

void check_file_changes()
{
    static std::filesystem::file_time_type last_check = std::chrono::file_clock::now();

    bool changed = false;
    for (auto x : header_files)
    {
        if (std::filesystem::last_write_time(x) > last_check || true)
        {
            search_definition(x);
            changed = true;
        }
    }

    for (auto x : code_files)
    {
        if (std::filesystem::last_write_time(x) > last_check || true)
        {
            search_load(x);
            changed = true;
        }
    }

    if (changed) link_with_classes();

    last_check = std::chrono::file_clock::now();
}

void run_prgm()
{
    setup_ast();

    check_file_changes();

    std::cout << '\n';

    for (auto x : loadable_processes)
    {
        if (x.second)
        {
            std::cout << x.second->name << "---\n";
            x.second->ast_expansion->use_editor();
        }
    }
}
