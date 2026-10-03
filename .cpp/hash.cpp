
#include "hash.hpp"

#include "factory.hpp"

#include "saver.hpp"
#include "loader.hpp"

bool hash::_register_hash()
{
  Factory ::_add_constructor([]() -> hash * { return new hash(); }, hash("hash"));
  return true;
}

void hash::save(Saver* saver)
{
    saver->save_data(value);
}

void hash::load(Loader* loader)
{
    value = loader->load_data<hash_t>();
}