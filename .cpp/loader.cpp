
#include "loader.hpp"

Loader::~Loader()
{
    if (values) delete values;
}

Loader::Loader(const char* where)
{
    open(where);
}

Loader::Loader(std::string where)
{
    open(where);
}

void Loader::open(const char* where)
{
    file = std::ifstream(where, std::ios::binary);
}

void Loader::open(std::string where)
{
    file = std::ifstream(where, std::ios::binary);
}

// Enables repeat loading after the file has been read through
void Loader::repeat()
{
    values = new tube;
}