
#pragma once

#ifndef PROCESS_H
#define PROCESS_H

#include "game.h"

#include "pos.h"

class Process
{
protected:
    Process* parent = nullptr;

    std::vector<Process*> children = {};

public:
    virtual void process(double delta);

    void process_children(double delta);

    pos get_parent_offset();

    virtual pos get_offset();

    virtual pos get_global_position();

    virtual double get_angle();

    virtual double get_global_angle();

    void add_child(Process* child);

    void set_parent(Process* new_parent);

    Process* get_parent();
};

#endif