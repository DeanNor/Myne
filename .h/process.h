
#pragma once

#include "game.h"

#include "pos.h"

class Process
{
protected:
    Process* parent = nullptr;

    std::vector<Process*> children = {};

public:
    virtual ~Process(){};

    virtual void process(double delta);

    void process_children(double delta);

    void add_child(Process* child);

    virtual void set_parent(Process* new_parent);

    Process* get_parent();
};