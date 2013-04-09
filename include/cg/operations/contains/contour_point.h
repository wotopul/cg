#pragma once

#include <cg/primitives/contour.h>
#include <cg/primitives/point.h>

namespace cg
{
   // c is convex contour ccw orientation
   inline bool convex_contains(contour_2 const & c, point_2 const & q);

   // c is ordinary contour
   inline bool contains(contour_2 const & c, point_2 const & q);
}
