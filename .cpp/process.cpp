
#include "process.hpp"

Process::~Process()
{
    if (parent != nullptr)
    {
        parent->remove_child(this);
    }
}

void Process::start_delete()
{
    for (Process* child : children)
    {
        child->start_delete();
    }

    get_current_game()->deletes.push_back(this);
}

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

void Process::remove_child(Process* child)
{
    std::vector<Process*>::iterator index = std::find(children.begin(), children.end(), child);

    if (index != children.end())
    {
        children.erase(index);
    }

    else
    {
        std::cout << "Not FOUND for remove child" << std::endl;// Error, object seems already deleted, or the parent is wrong
    }
}

void Process::set_parent(Process* new_parent)
{
    parent = new_parent;
}

Process* Process::get_parent()
{
    return parent;
}