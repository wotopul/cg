#pragma once

#include <cg/primitives/point.h>
#include <cg/operations/orientation.h>
#include <cg/primitives/triangle.h>
#include <cg/io/point.h>

#include <iostream>
#include <memory>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace cg
{


template <class Scalar> struct edge;
template <class Scalar> using edge_p = boost::shared_ptr< edge<Scalar> >;
template <class Scalar> using edge_wp = boost::weak_ptr< edge<Scalar> >;

template <class Scalar> struct vertex;
template <class Scalar> using vertex_p = boost::shared_ptr< vertex<Scalar> >;
template <class Scalar> using vertex_wp = boost::weak_ptr< vertex<Scalar> >;

template <class Scalar> struct face;
template <class Scalar> using face_p = boost::shared_ptr< face<Scalar> >;
template <class Scalar> using face_wp = boost::weak_ptr< face<Scalar> >;

template <class Scalar>
struct vertex
{
   bool inf;
   point_2t<Scalar> p;
   edge_wp<Scalar> out;

   vertex() : inf(true) {} // construct inf point
   vertex(point_2t<Scalar> p) : inf(false), p(p)  {}

   bool operator<(const vertex_p<Scalar> rhs) const
   {
      return p < rhs->p;
   }
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

   bool contains_inf() const { return begin->inf || end()->inf; }

   bool operator<(const edge<Scalar> rhs) const
   {
      point_2t<Scalar> a = begin->p;
      point_2t<Scalar> b = next->begin->p;
      point_2t<Scalar> c = rhs.begin->p;
      point_2t<Scalar> d = rhs.next->begin->p;
      return cg::orientation(a, b, b + (d - c)) == cg::CG_RIGHT;
   }
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
      auto curr = side;
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
         if (!curr->contains_inf() &&
             cg::orientation(curr->begin->p, curr->end()->p, p) != cg::CG_LEFT)
            return false;
         curr = curr->next;
      }
      return true;
   }
};

template <class Scalar>
face_p<Scalar> make_face(vertex_p<Scalar> a, vertex_p<Scalar> b, vertex_p<Scalar> c)
{
   face_p<Scalar> f = boost::make_shared< face<Scalar> >();
   edge_p<Scalar> edges[3] = {boost::make_shared< edge<Scalar> >(a),
                              boost::make_shared< edge<Scalar> >(b),
                              boost::make_shared< edge<Scalar> >(c)};
   for (int i = 0; i < 3; i++)
   {
      edges[i]->in_face = face_p<Scalar>(f);
      edges[i]->begin->out = edges[i];
   }
   f->side        = edges[0];
   edges[0]->next = edges[1];
   edges[1]->next = edges[2];
   edges[2]->next = f->side;
   return f;
}

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
      out << "inf";
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


}
