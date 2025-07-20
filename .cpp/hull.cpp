
#include "hull.hpp"
#include "box2d/box2d.h"

#include <fstream>

hull::hull(b2BodyId collision_body)
{
    static const int cap = 0xff;

    b2ShapeId shapes[cap];
    const int total = b2Body_GetShapes(collision_body, shapes, cap);

    for(int x = 0; x < total; ++x)
    {
        const b2ShapeId current = shapes[x];

        const b2Polygon poly = b2Shape_GetPolygon(current);

        std::array<pos, B2_MAX_POLYGON_VERTICES> shape;
        for (int y = 0; y < poly.count; ++y)
        {
            shape.at(y) = poly.vertices[y];
        }

        data.push_back(std::pair<int, std::array<pos, B2_MAX_POLYGON_VERTICES>>(poly.count, shape));
    }
}

void hull::save(std::string location)
{
    std::ofstream file(location);

    save(file);

    file.close();
}

void hull::save(std::ostream& where)
{
    // TODO save b2 data for shape 
    for (std::pair<int, std::array<pos, B2_MAX_POLYGON_VERTICES>> shape : data)
    {
        for (int z = 0; z < shape.first; ++z)
        {
            where << shape.second.at(z).x << '\b' << shape.second.at(z).y << '\b';
        }

        where << '\a';
    }
}

void hull::load(std::string location)
{
    std::ifstream file(location);

    load(file);

    file.close();
}

void hull::load(std::istream& where)
{
    double x = 0,y = 0;
    char cont = '\0';

    std::array<pos, B2_MAX_POLYGON_VERTICES> points;
    int used_points = 0;
    while (!where.eof() && used_points < 8)
    {
        where >> x;
        where.get();
        where >> y;
        where.get();
        
        cont = where.peek();

        points.at(used_points) = pos(x,y);

        ++used_points;

        if (cont == '\a')
        {
            data.push_back(std::pair<int, std::array<pos,B2_MAX_POLYGON_VERTICES>>(used_points, points));

            points = {};
            used_points = 0;

            where.get();
        }
    }
}