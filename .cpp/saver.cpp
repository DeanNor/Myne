
#include "saver.hpp"

Saver::Saver(const char* where)
{
    open(where);
}

Saver::Saver(std::string where)
{
    open(where);
}

void Saver::open(const char* where)
{
    file = std::ofstream(where, std::ios::binary);
}

void Saver::open(std::string where)
{
    file = std::ofstream(where, std::ios::binary);
}