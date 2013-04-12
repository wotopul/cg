#pragma once
#include <cg/primitives/rectangle.h>
#include <cg/primitives/triangle.h>
#include <cg/primitives/point.h>
#include <cg/primitives/segment.h>
#include <cg/operations/has_intersection/triangle_segment.h>

namespace cg {
   inline bool has_intersection(rectangle_2 const& rectangle, segment_2 const& segment) {
      point_2 beg = segment[0];
      point_2 end = segment[1];


      point_2 lb(rectangle.corner(0, 0)), rt(rectangle.corner(1, 1));

      if ((beg.x < lb.x && end.x < lb.x)
            || (beg.x > rt.x && end.x > rt.x)
            || (beg.y < lb.y && end.y < lb.y)
            || (beg.y > rt.y && end.y > rt.y))
      {
         return false;
      }

      if (rectangle.contains(beg) || rectangle.contains(end))
      {
         return true;
      }


      triangle_2 tr1(lb, rectangle.corner(1, 0), rt);
      triangle_2 tr2(lb, rectangle.corner(0, 1), rt);

      return has_intersection(tr1, segment) || has_intersection(tr2, segment);
   }
}
