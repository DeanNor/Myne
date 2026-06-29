
#include ".hpp/factory.hpp"
#include <stdexcept>
#include <string>

void* Factory::construct(hash name)
{
    try
    {
        return get_complex_constructors().at(name)();
    }

    catch (...)
    {
        throw std::runtime_error(std::string("Bad Type Data in input file / Unknown Type(Complex) Data in input file. ID: ") + std::to_string(name.value));
    }
}

Process* Factory::construct_process(const hash name)
{
    try
    {
        return get_process_constructors().at(name)();
    }

    catch (...)
    {
        throw std::runtime_error(std::string("Bad Type Data in input file / Unknown Type(Process) Data in input file. ID: ") + std::to_string(name.value));
    }
}