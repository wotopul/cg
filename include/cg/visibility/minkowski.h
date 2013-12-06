#pragma once

#include <cg/primitives/point.h>
#include <cg/primitives/vector.h>
#include <cg/primitives/contour.h>
#include <cg/operations/orientation.h>

#include <boost/utility.hpp>

namespace cg
{

template <class Scalar>
orientation_t last_orientation(const contour_2t<Scalar> & c, const point_2t<Scalar> & p)
{
   size_t size = c.size();
   assert(size >= 2);
   return orientation(c[size - 2], c[size - 1], p);
}

template <class Scalar>
contour_2t<Scalar> minkowski_sum(const contour_2t<Scalar> & a, const contour_2t<Scalar> & b)
{
   contour_2t<Scalar> res;
   auto a_circ = a.circulator( std::min_element(a.begin(), a.end()) );
   auto b_circ = b.circulator( std::min_element(b.begin(), b.end()) );
   for (size_t i = 0; i < a.size() + b.size(); i++)
   {
      point_2t<Scalar> p = *b_circ + vector_2t<Scalar>(a_circ->x, a_circ->y);
      if (res.size() >= 2 && last_orientation(res, p) == CG_COLLINEAR)
         res.remove_last();
      res.add_point(p);

      if (orientation4(*a_circ, *boost::next(a_circ), *b_circ, *boost::next(b_circ)) == CG_LEFT)
      {
         a_circ++;
      }
      else
      {
         b_circ++;
      }
   }

   while(res.size() > 2 && last_orientation(res, res[0]) == CG_COLLINEAR)
      res.remove_last();
   return res;
}

}
