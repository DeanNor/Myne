
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