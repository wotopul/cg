#pragma once
#include "orientation.h"
#include <cg/primitives/point.h>
#include <cg/primitives/segment.h>
#include <cg/primitives/triangle.h>
#include <cg/primitives/rectangle.h>
#include <cg/primitives/contour.h>

namespace cg
{

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

   namespace detail
   {
      inline bool test_orientation(orientation_t o)
      {
         return o == CG_COLLINEAR || o == CG_RIGHT;
      }

      enum class test_res_t
      {
         INTERSECTION, NO_INTERSECTION, ON_SEGMENT
      };

      inline test_res_t test_intersection_left_ray(point_2 const& point,
            point_2 seg_begin,
            point_2 seg_end)
      {
         if (seg_begin.x > point.x && seg_end.x > point.x)
         {
            return test_res_t::NO_INTERSECTION;
         }

         if (seg_begin == point || seg_end == point)
         {
            return test_res_t::ON_SEGMENT;
         }

         if (seg_begin.y > seg_end.y)
         {
            std::swap(seg_begin, seg_end);
         }

         if (seg_begin.x < point.x && seg_end.x < point.x)
         {
            return (seg_begin.y <= point.y && seg_end.y > point.y)
                   ? test_res_t::INTERSECTION : test_res_t::NO_INTERSECTION;
         }

         if (seg_end.y == seg_begin.y && seg_begin.y == point.y)
         {
            return test_res_t::ON_SEGMENT;
         }

         segment_2 contour_segment(seg_begin, seg_end);
         segment_2 aux_segment(point, {(std::min(seg_begin, seg_end)).x, point.y});

         return (seg_end.y != point.y && have_intersection(contour_segment, aux_segment))
                ? test_res_t::INTERSECTION : test_res_t::NO_INTERSECTION;
      }
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

   /**
    * @brief Checks if the point is inside the convex contour oriented clockwise
    * @param contour Convex clockwise oriented contour
    * @param point Point that should be checked
    * @return True if point is inside the contour, false otherwise
    */
   inline bool point_in_convex_contour(contour_2 const& contour,
                                       point_2 const& point)
   {
      auto first = contour[0];

      if (orientation(first, contour[1], point) == CG_LEFT)
      {
         return false;
      }

      auto itr = std::lower_bound(contour.begin() + 2, contour.end(), point,
                                  [&] (point_2 const& l, point_2 const& val)
      {
         auto o = orientation(first, l, val);
         return o == CG_RIGHT;
      });

      if (itr == contour.end())
      {
         return false;
      }

      return orientation(*(itr - 1), *itr, point) != CG_LEFT;
   }

   inline bool point_in_contour(contour_2 const& contour, point_2 const& val)
   {
      auto circulator = contour.circulator();
      int intersection_count = 0;

      for (size_t i = 0; i < contour.vertices_num(); ++i)
      {
         auto const& p1 = *circulator;
         ++circulator;
         auto const& p2 = *circulator;
         using namespace detail;

         switch (test_intersection_left_ray(val, p1, p2))
         {
         case test_res_t::INTERSECTION:
            intersection_count++;
            break;

         case test_res_t::ON_SEGMENT:
            return true;

         default:
            break;
         }
      }

      return intersection_count % 2 == 1;
   }
}
