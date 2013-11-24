#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/weak_ptr.hpp>

#include <cg/triangulation/triangulation_predicate.h>
#include <cg/operations/orientation.h>
#include <cg/primitives/point.h>
#include <cg/primitives/triangle.h>
#include <cg/io/point.h>

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
   face_p<Scalar>  in_face;

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
         if (!curr->contains_inf() &&
             cg::orientation(curr->begin->p, curr->end()->p, p) == cg::CG_RIGHT)
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


template <class Scalar>
class delaunay_triangulation
{
   std::vector< face_p<Scalar> > faces;
   std::vector< vertex_p<Scalar> > vertices;

   void init_triangulation();
   edge_p<Scalar> localize(point_2t<Scalar> p, std::vector<size_t> & containing);
   void split_up(std::vector<size_t> & containing, edge_p<Scalar> start, vertex_p<Scalar> v);
   void flip_if(edge_p<Scalar> e);
   bool non_delaunay(edge_p<Scalar> e);

   void reset();

public:

   delaunay_triangulation()
   {
      // add inf point
      vertices.push_back( boost::make_shared< vertex<Scalar> >() );
   }

   ~delaunay_triangulation();

   void clear()
   {
      reset();
      faces.clear();
      vertices.clear();
      vertices.push_back( boost::make_shared< vertex<Scalar> >() );
   }

   void add(point_2t<Scalar> p);
   void flip(edge_p<Scalar> e);

   template <class OutIter>
   OutIter triangulate(OutIter out) const
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
void delaunay_triangulation<Scalar>::reset()
{
   for (auto f = faces.begin(); f != faces.end(); f++)
   {
      (*f)->side->next.reset();
      (*f)->side->twin.reset();
      (*f)->side->begin.reset();
      (*f)->side->in_face.reset();
      (*f)->side.reset();
   }
}

template <class Scalar>
delaunay_triangulation<Scalar>::~delaunay_triangulation()
{
   reset();
}

template <class Scalar>
void delaunay_triangulation<Scalar>::init_triangulation()
{
   faces.push_back( make_face(vertices[0], vertices[1], vertices[2]) );
   faces.push_back( make_face(vertices[1], vertices[0], vertices[2]) );
   auto e1 = faces[0]->side, e2 = faces[1]->side;
   make_twins(e1, e2);
   make_twins(e1->next, e2->next->next);
   make_twins(e2->next, e1->next->next);
}

template <class Scalar>
bool delaunay_triangulation<Scalar>::non_delaunay(edge_p<Scalar> e)
{
   if (e->contains_inf() || e->twin->next->end()->inf)
      return false;
   return cg::in_circle(e->begin->p, e->next->begin->p, e->next->next->begin->p,
                        e->twin->next->end()->p);
}

template <class Scalar>
void delaunay_triangulation<Scalar>::flip(edge_p<Scalar> e)
{
   edge_p<Scalar> flipped = boost::make_shared< edge<Scalar> >(e->next->next->begin);
   edge_p<Scalar> flipped_twin = boost::make_shared< edge<Scalar> >(e->twin->next->next->begin);
   make_twins(flipped, flipped_twin);

   face_p<Scalar> flip_in = e->in_face;
   flip_in->side = e->next;
   e->twin->next->next->in_face = flip_in;
   flipped->in_face = flip_in;

   face_p<Scalar> twin_flip_in = e->twin->in_face;
   twin_flip_in->side = e->twin->next;
   e->next->next->in_face = twin_flip_in;
   flipped_twin->in_face = twin_flip_in;

   flipped->next = e->twin->next->next;
   flipped_twin->next = e->next->next;

   e->next->next->next = e->twin->next;
   e->next->next = flipped;
   e->twin->next->next->next = e->next;
   e->twin->next->next = flipped_twin;
}

template <class Scalar>
void delaunay_triangulation<Scalar>::flip_if(edge_p<Scalar> e)
{
   if (!non_delaunay(e))
      return;
   edge_p<Scalar> next1 = e->twin->next;
   edge_p<Scalar> next2 = next1->next;
   flip(e);
   flip_if(next1);
   flip_if(next2);
}

// returns edge of chain used in split_up
template <class Scalar>
edge_p<Scalar> delaunay_triangulation<Scalar>::localize(point_2t<Scalar> p, std::vector<size_t> & containing)
{
   edge_p<Scalar> start;
   for (size_t i = 0; i < faces.size(); i++)
   {
      if (!faces[i]->contains(p))
         continue;

      containing.push_back(i);
      edge_p<Scalar> curr = faces[i]->side;
      for (int j = 0; j < 3; j++)
      {
         if (!curr->contains_inf() &&
             cg::orientation(curr->begin->p, curr->end()->p, p) == cg::CG_LEFT)
         {
            if (start == nullptr)
               start = curr;
            else if ( (*curr) < (*start) )
               start = curr;
         }
         curr = curr->next;
      }
   }
   return start;
}

template <class Scalar>
void delaunay_triangulation<Scalar>::split_up(std::vector<size_t> & containing,
                                              edge_p<Scalar> start, vertex_p<Scalar> v)
{
   // build up chain
   std::vector< edge_p<Scalar> > chain;
   if (faces[containing.front()]->contains_inf() && faces[containing.back()]->contains_inf())
   {
      //std::cout << "out of convex" << std::endl;
      edge_p<Scalar> curr = start;
      for (size_t i = 0; i < containing.size() - 1; i++)
      {
         chain.push_back(curr);
         curr = curr->next->twin->next;
      }
      chain.push_back(curr);
      chain.push_back(curr->next);
      chain.push_back(start->next->next);
   }
   else if (containing.size() == 1)
   {
      //std::cout << "in convex inside triangle" << std::endl;
      edge_p<Scalar> curr = faces[containing.front()]->side;
      for (int i = 0; i < 3; i++)
      {
         chain.push_back(curr);
         curr = curr->next;
      }
   }
   else
   {
      //std::cout << "on edge" << std::endl;
      edge_p<Scalar> common;
      auto e1 = faces[containing[0]]->side;
      for (int i = 0; i < 3; i++)
      {
         auto e2 = faces[containing[1]]->side;
         for (int j = 0; j < 3; j++)
         {
            if (e2->twin == e1)
               common = e1;
            e2 = e2->next;
         }
         e1 = e1->next;
      }
      chain.push_back(common->next);
      chain.push_back(common->next->next);
      chain.push_back(common->twin->next);
      chain.push_back(common->twin->next->next);
   }

   // remove faces
   size_t back = faces.size() - 1;
   for (std::vector<size_t>::reverse_iterator i = containing.rbegin(); i != containing.rend(); i++)
   {
      std::swap(faces[*i], faces[back--]);
   }
   faces.resize(back + 1);

   size_t ch_length = chain.size();
   std::vector< edge_p<Scalar> > new_edges(ch_length);
   std::vector< face_p<Scalar> > new_faces(ch_length);

   for (size_t i = 0; i < ch_length; i++)
   {
      edge_p<Scalar> new_edge = boost::make_shared< edge<Scalar> >(v);
      edge_p<Scalar> new_twin = boost::make_shared< edge<Scalar> >(chain[i]->begin);
      new_faces[i] = boost::make_shared< face<Scalar> >();

      make_twins(new_edge, new_twin);
      new_edges[i] = new_edge;
      new_faces[i]->side = new_edges[i];
      faces.push_back(new_faces[i]);
   }

   for (size_t i = 0; i < ch_length; i++)
   {
      new_edges[i]->next = chain[i];
      new_edges[i]->twin->next = new_edges[(i + ch_length - 1) % ch_length];
      chain[i]->next = new_edges[(i + 1) % ch_length]->twin;

      new_edges[i]->in_face = new_faces[i];
      new_edges[i]->twin->in_face = new_faces[(i + ch_length - 1) % ch_length];
      chain[i]->in_face = new_faces[i];
   }

   for (edge_p<Scalar> e : chain)
   {
      flip_if(e);
   }
}

template <class Scalar>
void delaunay_triangulation<Scalar>::add(point_2t<Scalar> p)
{
   if (std::any_of(vertices.begin(), vertices.end(), [&p](vertex_p<Scalar> v){ return v->p == p; })) {
      return;
   }

   vertex_p<Scalar> v = boost::make_shared< vertex<Scalar> >(p);
   vertices.push_back(v);
   if (vertices.size() < 3)
      return;
   if (vertices.size() == 3)
   {
      init_triangulation();
      return;
   }

   // localization -- O(in faces.size()) here :-(
   std::vector<size_t> containing; // faces that contains added point
   edge_p<Scalar> start = localize(p, containing);

   split_up(containing, start, v);
}


}
