
#pragma once

#include "game.hpp"

#include "pos.hpp"

#include <memory>

class Process
{
protected:
    Process* parent = nullptr;

    std::vector<Process*> children = {};

    std::string name;

    bool to_delete = false;

public:
    virtual ~Process();

    virtual void _process(double delta);

    virtual void process(double delta);

    void process_children(double delta);

    void add_child(Process* child);

    void remove_child(Process* child);

    size_t get_total_children();

    size_t get_sum_total_children();

    std::vector<Process*> get_named_children(std::string term);

    virtual void set_parent(Process* new_parent);

    Process* get_parent();

    void start_delete();

    bool is_to_delete();

    void set_name(std::string new_name);

    std::string get_name();
};
