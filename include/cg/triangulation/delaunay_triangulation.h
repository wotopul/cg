#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/shared_ptr.hpp>

#include <cg/operations/orientation.h>
#include <cg/primitives/point.h>
#include <cg/primitives/triangle.h>
#include <cg/io/point.h>

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
    vertex(point_2t<Scalar> p) : inf(false), p(p)  {}
};

template <class Scalar>
struct edge
{
    vertex_p<Scalar> begin;
    edge_p<Scalar>   next;
    edge_p<Scalar>   twin;
    face_p<Scalar>   in_face;

    edge() {}
    edge(vertex_p<Scalar> begin)
        : begin(begin)
    {}

    vertex_p<Scalar> end() const { return next->begin; }
};

template <class Scalar>
void make_twins(edge_p<Scalar> e1, edge_p<Scalar> e2)
{
    e1->twin = e2;
    e2->twin = e1;
}

template <class Scalar>
struct face
{
    edge_p<Scalar> side;

    face() {}

    face(vertex_p<Scalar> a, vertex_p<Scalar> b, vertex_p<Scalar> c)
    {
        edge_p<Scalar> edges[3] = {edge_p<Scalar>(new edge<Scalar>(a)),
                                   edge_p<Scalar>(new edge<Scalar>(b)),
                                   edge_p<Scalar>(new edge<Scalar>(c))};
        for (int i = 0; i < 3; i++)
        {
            edges[i]->in_face = face_p<Scalar>(this);
        }
        side           = edges[0];
        edges[0]->next = edges[1];
        edges[1]->next = edges[2];
        edges[2]->next = side;
    }

    cg::triangle_2t<Scalar> to_triangle() const
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

    bool contains_inf() const
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

    bool contains(point_2t<Scalar> p) const
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
struct contains_pred
{
    point_2t<Scalar> p;

    contains_pred(point_2t<Scalar> p) : p(p) {}
    bool operator()(const face_p<Scalar> f)
    {
        return f->contains(p);
    }
};


// logging
template <class Scalar>
std::ostream & operator<<(std::ostream & out, const vertex_p<Scalar> v)
{
    if (v->inf)
        out << "inf" << std::endl;
    else
        out << "(" << v->p.x << ", " << v->p.y << ")";
    return out;
}

template <class Scalar>
std::ostream & operator<<(std::ostream & out, const edge_p<Scalar> e)
{
    out << "[ " << e->begin << ", " << e->end() << " ]";
    return out;
}

template <class Scalar>
std::ostream & operator<<(std::ostream & out, const face_p<Scalar> f)
{
    out << "{face: "     << " " << f->side << " "
        << f->side->next << " " << f->side->next->next << "}";
    return out;
}


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
    faces.push_back( face_p<Scalar>(new face<Scalar>(vertices[0], vertices[1], vertices[2])) );
    faces.push_back( face_p<Scalar>(new face<Scalar>(vertices[1], vertices[0], vertices[2])) );
    auto e1 = faces[0]->side, e2 = faces[1]->side;
    make_twins(e1, e2);
    make_twins(e1->next, e2->next->next);
    make_twins(e2->next, e1->next->next);
    std::cout << faces[0] << std::endl << faces[1] << std::endl;
}

template <class Scalar>
void delaunay_triangulation<Scalar>::add(point_2t<Scalar> p)
{
    std::cout << "adding point: " << p << std::endl;
    vertex_p<Scalar> v(new vertex<Scalar>(p));
    vertices.push_back(v);
    if (vertices.size() < 3)
        return;
    if (vertices.size() == 3)
    {
        init_triangulation();
        return;
    }

    // localization -- O(faces.size()) here :-(
    std::vector< face_p<Scalar> > containing; // faces that contains added point
    std::for_each(faces.begin(), faces.end(),
        [&containing, &p](face_p<Scalar> f)
            {
                if (f->contains(p))
                {
                    std::cout << f << " contains " << p << std::endl;
                    containing.push_back(f);
                }
            });

    // add new edges in face
    // for every bad edge in old face recursively fix them
}

}
