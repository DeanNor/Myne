
#include ".hpp/hash.hpp"

#include ".hpp/factory.hpp"

#include ".hpp/saver.hpp"
#include ".hpp/loader.hpp"

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