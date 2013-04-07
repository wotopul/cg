#pragma once
#include "orientation.h"
#include <cg/primitives/point.h>
#include <cg/primitives/segment.h>
#include <cg/primitives/triangle.h>
#include <cg/primitives/rectangle.h>

namespace cg
{
   namespace detail
   {
      inline bool test_orientation(orientation_t o)
      {
         return o == CG_COLLINEAR || o == CG_RIGHT;
      }
   }


   inline bool have_intersection(segment_2 const& a, segment_2 const& b)
   {
      bool aeq = a[0] == a[1],
           beq = b[0] == b[1];

      if (aeq && beq)
      {
         return a[0] == b[0];
      }
      else if (aeq)
      {
         return orientation(b[0], b[1], a[0]) == CG_COLLINEAR;
      }
      else if (beq)
      {
         return orientation(a[0], a[1], b[0]) == CG_COLLINEAR;
      }

      orientation_t ab0 = orientation(a[0], a[1], b[0]);
      orientation_t ab1 = orientation(a[0], a[1], b[1]);
      orientation_t ba0 = orientation(b[0], b[1], a[0]);
      orientation_t ba1 = orientation(b[0], b[1], a[1]);

      if (ab0 != ab1 && ba0 != ba1)
      {
         return true;
      }

      if (ab0 == CG_COLLINEAR && ab1 == CG_COLLINEAR)
      {
         if (a[0] < b[0] && a[0] < b[1] && a[1] < b[0] && a[1] < b[1])
         {
            return false;
         }

         if (a[0] > b[0] && a[0] > b[1] && a[1] > b[0] && a[1] > b[1])
         {
            return false;
         }

         return true;
      }

      return false;
   }

   inline bool point_in_triangle(triangle_2 const& triangle, point_2 const& point)
   {
      orientation_t o1 = orientation(triangle.points[0], triangle.points[1], point);
      orientation_t o2 = orientation(triangle.points[1], triangle.points[2], point);
      orientation_t o3 = orientation(triangle.points[2], triangle.points[0], point);
      using namespace detail;

      return test_orientation(o1) && test_orientation(o2) && test_orientation(o3);
   }

   inline bool have_intersection(triangle_2 const& triangle, segment_2 const& segment)
   {
      if (point_in_triangle(triangle, segment[0]) || point_in_triangle(triangle, segment[1]))
      {
         return true;
      }

      return have_intersection(segment_2(triangle.points[0], triangle.points[1]), segment)
             || have_intersection(segment_2(triangle.points[0], triangle.points[2]), segment)
             || have_intersection(segment_2(triangle.points[1], triangle.points[2]), segment);
   }

   inline bool have_intersection(rectangle_2 const& rectangle, segment_2 const& segment)
   {
      point_2 lb(rectangle.corner(0, 0)), rt(rectangle.corner(1, 1));
      triangle_2 tr1(lb, rectangle.corner(1, 0), rt);
      triangle_2 tr2(lb, rectangle.corner(0, 1), rt);

      return have_intersection(tr1, segment) || have_intersection(tr2, segment);
   }

}
