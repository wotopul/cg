#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/weak_ptr.hpp>

#include <cg/primitives/segment.h>
#include <cg/operations/contains/segment_point.h>
#include <cg/triangulation/cell.h>
#include <cg/triangulation/triangulation_predicate.h>
#include <cg/operations/orientation.h>
#include <cg/primitives/point.h>
#include <cg/primitives/triangle.h>
#include <cg/io/point.h>

namespace cg
{


template <class Scalar>
class delaunay_triangulation
{
   std::vector< face_p<Scalar> > faces;
   std::vector< vertex_p<Scalar> > vertices;

   void init_triangulation();

   void insert(face_p<Scalar> f, vertex_p<Scalar> v);
   void insert(edge_p<Scalar> common, vertex_p<Scalar> v);

   boost::optional< face_p<Scalar> > find_face(point_2t<Scalar> p);
   boost::optional< edge_p<Scalar> > find_edge(point_2t<Scalar> p);

   void split_up(std::vector<size_t> & containing, edge_p<Scalar> start, vertex_p<Scalar> v);
   void flip_if(edge_p<Scalar> e);

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
   //std::cerr << "flipping edge: " << e << std::endl;
   edge_p<Scalar> next1 = e->twin->next;
   edge_p<Scalar> next2 = next1->next;
   flip(e);
   flip_if(next1);
   flip_if(next2);
}

template <class Scalar>
boost::optional< face_p<Scalar> > delaunay_triangulation<Scalar>::find_face(point_2t<Scalar> p)
{
   contains_pred<Scalar> pred(p);
   auto pos = std::find_if(faces.begin(), faces.end(), pred);
   if (pos == faces.end())
      return boost::none;
   else
      return *pos;
}

template <class Scalar>
boost::optional< edge_p<Scalar> > delaunay_triangulation<Scalar>::find_edge(point_2t<Scalar> p)
{
   for (auto f : faces)
   {
      edge_p<Scalar> curr = f->side;
      for (int i = 0; i < 3; i++)
      {
         if (!curr->contains_inf() &&
             contains( segment_2t<Scalar>(curr->begin->p, curr->end()->p), p ) )
         {
            return curr;
         }
         curr = curr->next;
      }
   }
   return boost::none;
}

template <class Scalar>
void delaunay_triangulation<Scalar>::insert(face_p<Scalar> f, vertex_p<Scalar> v)
{
   //
   //std::cerr << "lies in face " << std::endl;
   //std::cerr << f << std::endl;
   //
   std::vector< edge_p<Scalar> > chain;
   edge_p<Scalar> curr = f->side;
   for (int i = 0; i < 3; i++)
   {
      chain.push_back(curr);
      curr = curr->next;
   }
   size_t ch_length = chain.size();
   std::vector< edge_p<Scalar> > new_edges(ch_length);
   std::vector< face_p<Scalar> > new_faces(ch_length);
   new_faces[0] = f;
   new_faces[1] = boost::make_shared< face<Scalar> >();
   new_faces[2] = boost::make_shared< face<Scalar> >();
   faces.push_back(new_faces[1]);
   faces.push_back(new_faces[2]);
   //

   for (size_t i = 0; i < ch_length; i++)
   {
      edge_p<Scalar> new_edge = boost::make_shared< edge<Scalar> >(v);
      edge_p<Scalar> new_twin = boost::make_shared< edge<Scalar> >(chain[i]->begin);

      make_twins(new_edge, new_twin);
      new_edges[i] = new_edge;
      new_faces[i]->side = new_edges[i];
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
   //
   //std::cerr << "new faces" << std::endl;
   //for (auto f : faces) {
   //   std::cerr << f << std::endl;
   //}
   //
}

template <class Scalar>
void delaunay_triangulation<Scalar>::insert(edge_p<Scalar> common, vertex_p<Scalar> v)
{
   face_p<Scalar> f1 = common->in_face;
   face_p<Scalar> f2 = common->twin->in_face;
   std::vector< edge_p<Scalar> > chain;
   chain.push_back(common->next);
   chain.push_back(common->next->next);
   chain.push_back(common->twin->next);
   chain.push_back(common->twin->next->next);
   size_t ch_length = chain.size();
   std::vector< edge_p<Scalar> > new_edges(ch_length);
   std::vector< face_p<Scalar> > new_faces(ch_length);
   new_faces[0] = f1;
   new_faces[1] = f2;
   new_faces[2] = boost::make_shared< face<Scalar> >();
   new_faces[3] = boost::make_shared< face<Scalar> >();
   faces.push_back(new_faces[2]);
   faces.push_back(new_faces[3]);
   //

   for (size_t i = 0; i < ch_length; i++)
   {
      edge_p<Scalar> new_edge = boost::make_shared< edge<Scalar> >(v);
      edge_p<Scalar> new_twin = boost::make_shared< edge<Scalar> >(chain[i]->begin);

      make_twins(new_edge, new_twin);
      new_edges[i] = new_edge;
      new_faces[i]->side = new_edges[i];
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
   //
   //std::cerr << "new faces" << std::endl;
   //for (auto f : faces) {
   //  std::cerr << f << std::endl;
   //}
   //
}

template <class Scalar>
void delaunay_triangulation<Scalar>::add(point_2t<Scalar> p)
{
   //
   //std::cerr << "---------------------------" << std::endl;
   //std::cerr << "add point: " << p << std::endl;
   //
   if (std::any_of(vertices.begin() + 1, vertices.end(), [&p](vertex_p<Scalar> v){ return v->p == p; })) {
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

   boost::optional< face_p<Scalar> > f = find_face(p);
   if (f)
   {
      insert(*f, v);
      return;
   }

   boost::optional< edge_p<Scalar> > e = find_edge(p);
   if (e)
   {
      insert(*e, v);
      return;
   }

   vertex_p<Scalar> near;
   if (vertices[1] < v)
      near = *std::max_element(vertices.begin() + 1, vertices.end() - 1);
   else
      near = *std::min_element(vertices.begin() + 1, vertices.end() - 1);
   vertex_p<Scalar> a = std::min(near, v), b = std::max(near, v);

   std::array<edge_p<Scalar>, 3> upper( {{boost::make_shared< edge<Scalar> >(a),
                                          boost::make_shared< edge<Scalar> >(b),
                                          boost::make_shared< edge<Scalar> >(vertices[0])}} );
   std::array<edge_p<Scalar>, 3> lower( {{boost::make_shared< edge<Scalar> >(b),
                                          boost::make_shared< edge<Scalar> >(a),
                                          boost::make_shared< edge<Scalar> >(vertices[0])}} );
   make_twins(upper[0], lower[0]);
   make_twins(upper[1], lower[2]);
   make_twins(upper[2], lower[1]);
   for (int i = 0; i < 3; i++)
   {
      upper[i]->next = upper[(i + 1) % 3];
      lower[i]->next = lower[(i + 1) % 3];
   }

   face_p<Scalar> up  = boost::make_shared< face<Scalar> >();
   face_p<Scalar> low = boost::make_shared< face<Scalar> >();
   up ->side = upper[0];
   low->side = lower[0];
   for (int i = 0; i < 3; i++)
   {
      upper[i]->in_face = up;
      lower[i]->in_face = low;
   }
   faces.push_back(up);
   faces.push_back(low);

   //
   //std::cerr << "new faces" << std::endl;
   //for (auto f : faces) {
   //  std::cerr << f << std::endl;
   //}
   //
}


}
