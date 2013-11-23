#pragma once

#include <boost/shared_ptr.hpp>

#include <cg/primitives/point.h>
#include <cg/primitives/triangle.h>

namespace cg
{
using boost::shared_ptr;

template <class Scalar>
struct edge;

template <class Scalar>
struct vertex
{
    vertex() {}

    shared_ptr< edge<Scalar> > out;
};

template <class Scalar>
struct edge
{
    edge() {}

    shared_ptr< vertex<Scalar> > begin;
    shared_ptr< edge<Scalar> > next;
    shared_ptr< edge<Scalar> > twin;
};

template <class Scalar>
struct face
{
    face() {}

    shared_ptr< edge<Scalar> > side;
};

template <class Scalar>
class delaunay_triangulation
{
public:
    delaunay_triangulation() {}
    void add(point_2t<Scalar> p);

    template <class OutIter>
    OutIter triangulate(OutIter out)
    {
        *out++ = cg::triangle_2(point_2(1, 1), point_2(60, 60), point_2(60, 1));
        return out;
    }
};

}
