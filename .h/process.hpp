
#pragma once

#include "game.hpp"

#include "pos.hpp"

#include <algorithm>

class Process
{
protected:
    Process* parent = nullptr;

    std::vector<Process*> children = {};

public:
    virtual ~Process();

    void start_delete();

    virtual void process(double delta);

    void process_children(double delta);

    void add_child(Process* child);

    void remove_child(Process* child);

    virtual void set_parent(Process* new_parent);

    Process* get_parent();
};
