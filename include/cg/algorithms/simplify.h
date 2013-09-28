#pragma once

#include <cg/primitives/point.h>
#include <cg/primitives/contour.h>
#include <vector>

namespace cg {
   namespace detail {
      inline double self_scalar_product(point_2 const& a, point_2 const& b)
      {
         auto p = a.x - b.x;
         auto q = a.y - b.y;

         return p * p + q * q;
      }

      inline double dist(point_2 const& a, point_2 const& b)
      {
         auto p = a.x - b.x;
         auto q = a.y - b.y;
         return sqrt(p * p + q * q);
      }

      inline double projection_on_seg(point_2 const& s, point_2 const& t, point_2 const& p)
      {
         auto x = (t.x - s.x) * (p.x - s.x);
         auto y = (t.y - s.y) * (p.y - s.y);
         return (x + y) / self_scalar_product(s, t);
      }

      inline double dist(point_2 const& seg_start, point_2 const& seg_end, point_2 const& p)
      {
         auto r = projection_on_seg(seg_start, seg_end, p);
         if (r < 0.)
            return dist(seg_start, p);
         else if (r <= 1.) {
            double l = (seg_end.x - seg_start.x) * (p.y - seg_start.y);
            double r = (seg_end.y - seg_start.y) * (p.x - seg_start.x);
            auto s = fabs(l - r) / 2.;

            return s / dist(seg_start, seg_end);
         } else {
            return dist(seg_end, p);
         }
      }

      template<class Itr, class OutItr>
      OutItr douglas_peucker_impl(Itr begin, Itr end, OutItr out, double eps)
      {
         end--;
         if (begin == end)
            return out;

         auto max_dist = 0.;
         auto max_dist_itr = begin;
         for (auto i = std::next(begin); i != end; ++i)
         {
            auto d = dist(*begin, *end, *i);
            if (d > max_dist)
            {
               max_dist = d;
               max_dist_itr = i;
            }
         }
         if (max_dist <= eps) {
            *(out++) = *end;
            return out;
         } else
         {
            out = douglas_peucker_impl(begin, ++max_dist_itr, out, eps);
            out = douglas_peucker_impl(--max_dist_itr, ++end, out, eps);
            return out;
         }
      }
   }

   template<class Itr, class OutItr>
   OutItr simplify(Itr begin, Itr end, OutItr out, double eps)
   {
      if (begin == end)
         return out;

      *out++ = *begin;
      return detail::douglas_peucker_impl(begin, end, out, eps);
   }
}
