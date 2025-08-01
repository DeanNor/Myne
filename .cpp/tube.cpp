
#include ".hpp/tube.hpp"

void tube::update_index()
{
    if (index >= size)
    {
        index = 0;
    }
}

void* tube::get()
{
    void* d = values[index];

    index++;
    update_index();

    return d;
}

void tube::add(void* val)
{
    size++;
    values.push_back(val);
}

tube::~tube()
{
    for (void* d : values)
    {
        free(d);
    }
}