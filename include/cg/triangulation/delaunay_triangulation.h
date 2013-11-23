#pragma once

#include <vector>
#include <algorithm>
#include <boost/shared_ptr.hpp>

#include <cg/operations/orientation.h>
#include <cg/primitives/point.h>
#include <cg/primitives/triangle.h>

namespace cg
{


template <class Scalar> struct edge;
template <class Scalar>
using edge_p = boost::shared_ptr< edge<Scalar> >;

template <class Scalar> struct vertex;
template <class Scalar>
using vertex_p = boost::shared_ptr< vertex<Scalar> >;

template <class Scalar> struct face;
template <class Scalar>
using face_p = boost::shared_ptr< face<Scalar> >;

template <class Scalar>
struct vertex
{
    bool inf;
    point_2t<Scalar> p;
    edge_p<Scalar> out;

    vertex() {}
    vertex(point_2t<Scalar> p) : p(p), inf(false) {}
};

template <class Scalar>
struct edge
{
    vertex_p<Scalar> begin;
    edge_p<Scalar>   next;
    edge_p<Scalar>   twin;

    edge() {}
    edge(vertex_p<Scalar> begin)
        : begin(begin)
    {}

    vertex_p<Scalar> end() { return next->begin; }
};

template <class Scalar>
struct face
{
    edge_p<Scalar> side;

    face() {}

    face(vertex_p<Scalar> a, vertex_p<Scalar> b, vertex_p<Scalar> c)
    {
        side = new edge<Scalar>(a);
        side->next = new edge<Scalar>(b);
        side->next->next = new edge<Scalar>(c);
        side->next->next->next = side;
    }

    cg::triangle_2t<Scalar> to_triangle()
    {
        cg::triangle_2t<Scalar> res;
        auto curr = side;
        for (int i = 0; i < 3; i++)
        {
            res[i] = curr->begin->p;
            curr = curr->next;
        }
        return res;
    }

    bool contains_inf()
    {
        auto curr = side; // a bit of copy-paste here
        for (int i = 0; i < 3; i++)
        {
            if (curr->begin->inf)
                return true;
            curr = curr->next;
        }
        return false;
    }

    bool contains(point_2t<Scalar> p)
    {
        auto curr = side;
        for (int i = 0; i < 3; i++)
        {
            if (curr->begin->inf || curr->end()->inf)
                continue;
            if (cg::orientation(curr->begin->p, curr->end()->p, p) == cg::CG_RIGHT)
                return false;
            curr = curr->next;
        }
        return true;
    }
};

template <class Scalar>
class delaunay_triangulation
{
    std::vector< face_p<Scalar> > faces;
    std::vector< vertex_p<Scalar> > vertices;

    void init_triangulation();

public:

    delaunay_triangulation() {}
    void add(point_2t<Scalar> p);

    template <class OutIter>
    OutIter triangulate(OutIter out)
    {
        for (auto i = faces.begin(); i != faces.end(); i++)
        {
            if ( (*i)->contains_inf() )
                continue;
            *out++ = (*i)->to_triangle();
        }
        return out;
    }
};

template <class Scalar>
void delaunay_triangulation<Scalar>::init_triangulation()
{
    faces.push_back( new face<Scalar>(vertices[0], vertices[1], vertices[2]) );
    faces.push_back( new face<Scalar>(vertices[1], vertices[0], vertices[2]) );
    auto e1 = faces[0]->side, e2 = faces[1]->side;
    for (int i = 0; i < 3; i++)
    {
        e1->twin = e2;
        e2->twin = e1;
        e1 = e1->next;
        e2 = e2->next;
    }
}

template <class Scalar>
void delaunay_triangulation<Scalar>::add(point_2t<Scalar> p)
{
    vertex_p<Scalar> v(new vertex<Scalar>(p));
    vertices.push_back(v);
    if (vertices.size() < 3)
        return;
    if (vertices.size() == 3)
        init_triangulation();

    // locate
    // add new edges in face
    // for every bad edge in old face recursively fix them
}

}
