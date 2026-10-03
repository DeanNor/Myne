
#pragma once

#include "hash.hpp"

#include "sprite.hpp"

#include <map>

class ImageCache
{
private:
    std::map<hash_t, basic_sprite*> sprites;

    
};