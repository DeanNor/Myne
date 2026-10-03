
#pragma once

#include "saver.hpp"
#include "loader.hpp"

#include <cstddef>
#include <cstdlib>
#include <string>
#include <vector>

#include <box2d/id.h>
#include <box2d/types.h>
#include <ConcavePolygon.h>

#include "b2.h"

#include <iostream>

// Storer for both concave polygons and decomposed convex polygons. Only decomposed ones can be used by box2d, so call decompose_points() before physics use
struct hull
{
ASSIGN_VAR_CONSTRUCTOR(hull);

public:
    cxd::ConcavePolygon values;

    bool decomposed = false;

    bool error = false;

    static void decompose_large_poly(cxd::ConcavePolygon& large)
    {
        int point_count = large.getPointCount();
        if (point_count <= 8) return;

        large.slicePolygon(0, point_count / 2);

        for (auto& x : large.getSubPolygons())
        {
            decompose_large_poly(x);
        }
    }

public:
    hull() = default;

    hull(b2BodyId collision_body) : decomposed(true)
    {
        int cap = b2Body_GetShapeCount(collision_body);
        b2ShapeId* shape_list = (b2ShapeId*)std::malloc( cap * sizeof(b2ShapeId));

        b2Body_GetShapes(collision_body, shape_list, cap);

        for (int x = 0; x < cap; ++x)
        {
            b2Polygon poly = b2Shape_GetPolygon(shape_list[x]);

            cxd::ConcavePolygon c_poly;

            for (int y = 0; y < B2_MAX_POLYGON_VERTICES; y++)
            {
                c_poly.addPoint(cxd::Vec2{poly.vertices[y].x, poly.vertices[y].y});
            }

            values.addSubPolygon(c_poly);
        }

        free(shape_list);
    }

    hull(std::vector<cxd::Vertex> points) : values(points)
    {
        
    }

    [[nodiscard("Call decompose_points to convert this hull, this returns a decomposed version, but leaves the current hull alone")]]
    hull get_decomposed()
    {
        hull temp = *this;

        temp.decompose_points();

        return temp;
    }

    void decompose_points()
    {
        if (!decomposed)
        {
            if (values.getVertices().size() > 3)
            {
                if (values.selfIntersects())
                {
                    error = true;
                    return;
                }
            }

            values.convexDecomp();

            error = false;

            decompose_large_poly(values);

            decomposed = true;

            std::vector<cxd::ConcavePolygon> c_polys;
            values.returnLowestLevelPolys(c_polys);

            values.setSubPolygons(c_polys);
        }
    }

    void save(std::string location)
    {
        Saver s(location);

        save(&s);

        s.close();
    }

    void save(Saver* os) const
    {
        os->save_data(decomposed);

        if (decomposed)
        {
            std::vector<cxd::ConcavePolygon > convex_list;
            values.returnLowestLevelPolys(convex_list);

            os->save_data(convex_list.size());

            for (cxd::ConcavePolygon v : convex_list)
            {
                std::vector<cxd::Vertex> vertex_list = v.getVertices();
                os->save_data(vertex_list.size());
                for (cxd::Vertex vertex : vertex_list)
                {
                    os->save_data(vertex.position.x);
                    os->save_data(vertex.position.y);
                }
            }
        }

        else
        {
            std::vector<cxd::Vertex> vertex_list = values.getVertices();

            os->save_data(vertex_list.size());

            for (cxd::Vertex vertex : vertex_list)
            {
                os->save_data(vertex.position.x);
                os->save_data(vertex.position.y);
            }
        }
    }

    void load(std::string location)
    {
        Loader l(location);

        load(&l);

        l.close();
    }

    void load(Loader* os)
    {
        decomposed = os->load_data<bool>();

        if (decomposed)
        {
            size_t len = os->load_data<size_t>();

            for (size_t x = 0; x < len; ++x)
            {
                size_t vertex_len = os->load_data<size_t>();

                cxd::ConcavePolygon value;

                for (int y = 0; y < vertex_len; ++y)
                {
                    cxd::Vec2 val;
                    val.x = os->load_data<float>();
                    val.y = os->load_data<float>();

                    value.addPoint(val);
                }

                values.addSubPolygon(value);
            }
        }

        else
        {
            size_t vertex_len = os->load_data<size_t>();

            for (int y = 0; y < vertex_len; ++y)
            {
                cxd::Vec2 val;
                val.x = os->load_data<float>();
                val.y = os->load_data<float>();

                values.addPoint(val);
            }
        }
    }

    static void attach_to_body(const b2BodyId body, const b2ShapeDef* def, hull& shape)
    {
        if (!shape.decomposed)
        {
            shape.decompose_points();
        }

        std::vector<cxd::ConcavePolygon> vec;
        shape.values.returnLowestLevelPolys(vec);

        for (const cxd::ConcavePolygon& x : vec)
        {
            std::vector<cxd::Vertex> points = x.getVertices();

            // Things to note: cxd::Vertex is just a container/rename of cxd::Vec2. cxd::Vec2 is just an x,y float pair, which is exactly the same as what a b2Vec2 is. So convertable, hopefully!
            static_assert(sizeof(b2Vec2) == sizeof(cxd::Vertex), "Mismatch with cxd::Vertex size and b2Vec2 size");

            for (auto x : points)
            {
                std::cout << x.position.x << ' ' << x.position.y << '\n';
            }
            std::cout << '\n';
            
            b2Hull b_hull = b2ComputeHull((b2Vec2*)(&points.front()), points.size());

            b2Polygon v = b2MakePolygon(&b_hull, 0.0f);
            b2CreatePolygonShape(body, def, &v);
        }
    }
};
