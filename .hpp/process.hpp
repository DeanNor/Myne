
#pragma once

#include "loader.hpp"
#include "saver.hpp"

#include ".hpp/tfm.hpp"

class Process
{
ASSIGN_CONSTRUCTOR_OVERRIDE(Process);

protected:
    Process* parent = nullptr;

    std::vector<Process*> children = {};

    std::string name = std::string{"Undefined"};

    bool to_delete = false;

public:
    virtual ~Process();

    Process() = default;

    virtual void load(Loader* load);

    virtual void save(Saver* save) const;

    virtual void _process();

    virtual void process();

    void process_children();

    void add_child(Process* child);

    void remove_child(Process* child);

    Process* get_child(size_t index);

    std::vector<Process*> get_children();

    // Gets the number of direct children
    size_t get_total_children();

    // Gets the total number of children, including their children.
    size_t get_sum_total_children();

    std::vector<Process*> get_named_children(std::string term);

    // Most lilely looking for remove_child(). This does not do any removing
    virtual void set_parent(Process* new_parent);

    Process* get_parent();

    // Starts and queues deletion for the end of the frame _Safe_
    virtual void start_delete();

    // Immediately deletes the object _Extrodinarily Unsafe_ when the object is in the call tree and should only be called outside of the main loop or on parentless objects in the process function
    void del();
    
    bool is_to_delete();

    void set_name(std::string new_name);

    std::string get_name();
};

inline void Saver::save_process(Process* data)
{
    save_data<int64_t>(data->_get_type_hash().value);
    data->save(this);  
}

inline Process* Loader::load_process()
{
    if (eof)
    {
        if (values != nullptr)
        {
            hash* hash_name = (hash*)(values->get());

            Process* p = Factory::construct_process(*hash_name);

            p->load(this);

            return p;
        }

        else throw std::bad_alloc();
    }

    else
    {
        Process* p;

        if (values)
        {
            hash_t* hash_name = (hash_t*)malloc(sizeof(hash_t));

            file.read((char*)hash_name, sizeof(hash_t));

            values->add(hash_name);

            p = Factory::construct_process(*hash_name);
        }

        else
        {
            hash hash_name = load_data<hash_t>();
            p = Factory::construct_process(hash_name);
        }

        p->load(this);

        eof = file.eof();

        return p;
    }
}