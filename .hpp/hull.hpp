
#pragma once

#include "box2d/collision.h"
#include "box2d/id.h"

#include ".hpp/pos.hpp"

#include <iostream>
#include <vector>

struct hull
{
public:
    std::vector<std::pair<int,std::array<pos, B2_MAX_POLYGON_VERTICES>>> data;

    hull() = default;

    hull(b2BodyId collision_body);

    void save(std::string location);

    void save(std::ostream& where);

    void load(std::string location);

    void load(std::istream& where);
};