
#include "process.h"

void Process::process(double delta)
{
    process_children(delta);
}

void Process::process_children(double delta)
{
    for (Process* child : children)
    {
        child->process(delta);
    }
}

pos Process::get_parent_offset()
{
    if (parent != nullptr)
    {
        return parent->get_offset();
    }

    return pos();
}

pos Process::get_offset()
{
    return pos();
}

pos Process::get_global_position()
{
    return pos();
}

double Process::get_angle()
{
    return 0.0;
}

double Process::get_global_angle()
{
    return 0.0;
}

void Process::add_child(Process* child)
{
    children.push_back(child);
    child->set_parent(this);
}

void Process::set_parent(Process* new_parent)
{
    parent = new_parent;
}

Process* Process::get_parent()
{
    return parent;
}