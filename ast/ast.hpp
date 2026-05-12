
#pragma once

#include <chrono>
#include <cstdlib>
#include <filesystem>

#include "ast/ast_load.hpp"
#include "ast/ast_stuff.hpp"
#include "ast/ts_tool.h"

#include "ast/ast_custom.hpp"

#include <thread>
#include <unordered_map>

// TODO make all the error windows here, and in other ast_* files, present to a buffer to be displayed

inline std::vector<std::string> header_files = {".hpp/blendobj.hpp"};
inline std::vector<std::string> code_files = {".hpp/blendobj.hpp"};

inline std::vector<std::thread> active_searches;

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

inline void load_files()
{

}

inline void init_base_loads()
{
    // TODO in-library stuff

    load_process* process = new load_process("Process");
    base_loads_process.emplace(process->class_type.value,process);
    load_object* object = new load_object("Object");
    base_loads_process.emplace(object->class_type.value,object);
    load_drawobj* drawobj = new load_drawobj("DrawObj");
    base_loads_process.emplace(drawobj->class_type.value,drawobj);

    load_pos* pos_v = new load_pos("pos");
    base_loads_complex.emplace(pos_v->class_type.value,pos_v);
    load_rad* rad_v = new load_rad("rad");
    base_loads_complex.emplace(rad_v->class_type.value,rad_v);

    load_string* string = new load_string("std::string");
    base_loads_complex.emplace(string->class_type.value,string);
}

inline void check_file_changes()
{
    static std::filesystem::file_time_type last_check = std::chrono::file_clock::now();

    bool changed = false;
    for (auto x : header_files)
    {
        if (std::filesystem::last_write_time(x) > last_check || true) // TODO remove true
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
}

inline void run_prgm()
{
    setup_ast();

    check_file_changes();

    exit_ast();
}
