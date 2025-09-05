
#pragma once

#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <iostream>

#include "ast/ast_search.hpp"
#include "ast/ts_tool.h"

#include "ast/ast_load.hpp"

#include <thread>

// TODO make all the error windows here, and in other ast_* files, present to a buffer to be displayed

std::mutex file_lock;
std::vector<std::string> header_files;
std::vector<std::string> code_files;

std::vector<std::thread> active_searches;

void check_file_changes();

void init_base_loads();

void load_files();

inline void setup_ast()
{
    refs = construct_storer();

    init_base_loads();

    load_files();
}

inline void exit_ast()
{
    free(refs);
}

void load_files()
{
    // TODO load from file
    header_files = {"./.hpp/drawobj.hpp","./.hpp/pos.hpp","./.hpp/rad.hpp", "./main.cpp"};

    code_files = {"./.cpp/drawobj.cpp","./.cpp/pos.cpp","./.hpp/rad.hpp"};
}

void init_base_loads();

void check_file_changes()
{
    auto tim = std::chrono::high_resolution_clock::now();
    file_lock.lock();

    static std::filesystem::file_time_type last_check = std::chrono::file_clock::now();

    bool changed = false;
    for (auto x : header_files)
    {
        if (std::filesystem::last_write_time(x) > last_check || true)
        {
            active_searches.emplace_back(search_definition,x);
            changed = true;
        }
    }

    for (auto x : code_files)
    {
        if (std::filesystem::last_write_time(x) > last_check || true)
        {
            active_searches.emplace_back(search_load,x);
            changed = true;
        }
    }

    for (auto& thread : active_searches) // Wait this current thread until the remainder are finished
    {
        thread.join();
    }

    if (changed) link_with_classes();

    last_check = std::chrono::file_clock::now();

    file_lock.unlock();

    std::cout << std::chrono::high_resolution_clock::now() - tim << '\n';
}

void run_prgm()
{
    setup_ast();

    check_file_changes();
}
