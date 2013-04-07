#pragma once
#include <cg/primitives/point.h>
#include <cg/operations/orientation.h>
#include <algorithm>

namespace cg
{
   template<class Scalar>
   class triangle_2t
   {

   public:
      point_2t<Scalar> points[3];

      triangle_2t(point_2t<Scalar> const& p1, point_2t<Scalar> const& p2, point_2t<Scalar> const& p3)
         : points({p1, p2, p3})
      {
         auto min = std::min_element(points, points + 3);
         std::swap(points[0], *min);
         if (orientation(points[0], points[1], points[2]) == CG_LEFT) {
            std::swap(points[1], points[2]);
         }
      }
   };

   typedef triangle_2t<double> triangle_2;
}
