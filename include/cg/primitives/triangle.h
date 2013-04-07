#include <cg/primitives/point.h>
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
         std::sort(points, points + 3);
      }
   };

   typedef triangle_2t<double> triangle_2;
}
