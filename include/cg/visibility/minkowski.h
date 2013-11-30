#pragma once

#include <cg/primitives/point.h>
#include <cg/primitives/vector.h>
#include <cg/primitives/contour.h>
#include <cg/operations/orientation.h>

#include <boost/utility.hpp>

namespace cg
{

template <class Scalar>
contour_2t<Scalar> minkowski_sum(const contour_2t<Scalar> & a, const contour_2t<Scalar> & b)
{
   contour_2t<Scalar> res;
   auto a_circ = a.circulator( std::min_element(a.begin(), a.end()) );
   auto b_circ = b.circulator( std::min_element(b.begin(), b.end()) );
   while (res.size() < a.size() + b.size())
   {
      res.add_point(*b_circ + vector_2t<Scalar>(a_circ->x, a_circ->y));
      (orientation4(*a_circ, *boost::next(a_circ), *b_circ, *boost::next(b_circ)) == CG_LEFT ? a_circ : b_circ)++;
   }
   return res;
}

}
