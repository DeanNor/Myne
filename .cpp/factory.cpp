
#include ".hpp/factory.hpp"

void* Factory::construct(hash name)
{
    return get_complex_constructors().at(name)();
}

Process* Factory::construct_process(const hash name)
{
    return get_process_constructors().at(name)();
}