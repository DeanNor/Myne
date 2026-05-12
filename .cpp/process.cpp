
#include ".hpp/process.hpp"

#include ".hpp/err.hpp"
#include ".hpp/game.hpp"

Process::~Process()
{
    if (parent != nullptr)
    {
        parent->remove_child(this);
    }
}

void Process::load(Loader* load)
{
    name = load->load_complex<std::string>();

    size_t size = load->load_data<size_t>();
    children.reserve(size);

    for (size_t x = size_t(0); x < size; ++x)
    {
        children.push_back(load->load_process());
    }
}

void Process::save(Saver* save) const
{
    save->save_complex(name);

    save->save_data(children.size());

    for (Process* p : children)
    {
        save->save_process(p);
    }
}

void Process::_process()
{
    process();

    add_children();

    process_children();
}

void Process::process()
{
    return;
}

void Process::process_children()
{
    for (Process* child : children)
    {
        child->_process();
    }
}

void Process::add_child(Process* child)
{
    new_children.push_back(child);
    child->set_parent(this);
}

void Process::remove_child(Process* child)
{
    if (child)
    {
        child->set_parent(nullptr);

        std::vector<Process*>::iterator index = std::find(children.begin(), children.end(), child);

        if (index == children.end())
        {
            index = std::find(new_children.begin(), new_children.end(), child);

            ASSERT(index != new_children.end(), "Error with remove child");

            new_children.erase(index);
        }

        else children.erase(index);
    }
}

void Process::add_children()
{
    for (Process* x : new_children)
    {
        children.push_back(x);
    }

    new_children.clear();
}

Process* Process::get_child(size_t index)
{
    if (index > children.size())
    {
        return new_children.at(index - children.size());
    }

    return children.at(index);
}

std::vector<Process*>& Process::get_children()
{
    return children;
}

std::vector<Process*>& Process::get_new_children()
{
    return new_children;
}

size_t Process::get_total_children()
{
    return children.size() + new_children.size();
}

size_t Process::get_sum_total_children()
{
    size_t val = 0;

    for (Process* child : children)
    {
        val += child->get_sum_total_children() + 1;
    }

    for (Process* child : new_children)
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

    for (Process* child : new_children)
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

        for (Process* child : new_children)
        {
            child->start_delete();
        }

        get_current_game()->add_to_deletes(this);
    }
}

void Process::del()
{
    for (Process* child : children)
    {
        child->set_parent(nullptr);
        child->del();
    }

    for (Process* child : new_children)
    {
        child->set_parent(nullptr);
        child->del();
    }

    delete this;
}

bool Process::is_to_delete()
{
    return to_delete;
}

void Process::set_name(std::string new_name)
{
    name = new_name;
}

std::string Process::get_name()
{
    return name;
}