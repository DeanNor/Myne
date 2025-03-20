
#include "process.hpp"

Process::~Process()
{
    if (parent != nullptr)
    {
        parent->remove_child(this);
    }
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

size_t Process::get_total_children()
{
    return children.size();
}

size_t Process::get_sum_total_children()
{
    size_t val = 0;

    for (Process* child : children)
    {
        val += child->get_sum_total_children() + 1;
    }

    return val;
}

std::vector<Process*> Process::get_named_children(std::string term)
{
    std::vector<Process*> found_children;
    for (Process* child : children)
    {
        if (child->name == term)
        {
            found_children.push_back(child);
        }
    }

    return found_children;
}

void Process::set_parent(Process* new_parent)
{
    parent = new_parent;
}

Process* Process::get_parent()
{
    return parent;
}

void Process::start_delete()
{
    if (!to_delete)
    {
        to_delete = true;

        for (Process* child : children)
        {
            child->start_delete();
        }

        get_current_game()->deletes.push_back(this);
    }
}

bool Process::is_to_delete()
{
    return to_delete;
}