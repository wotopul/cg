#include <cmath>
#include "stdafx.h"

#include "cg/visualization/draw_util.h"

#include "cg/common/range.h"

#include "cg/io/point.h"

namespace cg {
namespace visualization
{
   void draw(drawer_type & drawer, rectangle_2f const & rect)
   {
      drawer.draw_line(rect.corner(0, 0), rect.corner(1, 0));
      drawer.draw_line(rect.corner(1, 0), rect.corner(1, 1));
      drawer.draw_line(rect.corner(1, 1), rect.corner(0, 1));
      drawer.draw_line(rect.corner(0, 1), rect.corner(0, 0));
   }

   void draw(drawer_type &drawer, triangle_2 const & tr)
   {
      drawer.draw_line(tr[0], tr[1]);
      drawer.draw_line(tr[1], tr[2]);
      drawer.draw_line(tr[0], tr[2]);
   }

   void draw(drawer_type & drawer, cg::point_2 const & center, double radius)
   {
      static const double PI = 3.14159265;
      cg::point_2 curr(center.x + radius, center.y);
      for (int angle = 0; angle <= 360; angle++)
      {
         double alpha = angle * PI / 180;
         cg::point_2 next(center.x + radius * cos(alpha), center.y + radius * sin(alpha));
         drawer.draw_line(curr, next);
         curr = next;
      }
   }

   void draw(drawer_type & drawer, contour_2f const & cnt, bool draw_vertices)
   {
      contour_circulator_f beg(cnt), it = beg;

      do
      {
         point_2f pt = *it;
         if (draw_vertices)
            drawer.draw_point(pt, 3);
         ++it;
         drawer.draw_line(segment_2f(pt, *it));
      }
      while (it != beg);
   }
}}
