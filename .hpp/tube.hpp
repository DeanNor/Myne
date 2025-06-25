
#pragma once

#include <cstdlib>
#include <vector>

// More of a racetrack, loops through the list, getting the next element with .get() and looping at the end of the elements
struct tube
{
    void update_index();

public:
    std::vector<void*> values;
    size_t index = 0;
    size_t size = 0;

    void* get();

    void add(void* val);

    ~tube();
};