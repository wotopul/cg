#pragma once

#include <cg/primitives/segment.h>
#include <cg/primitives/contour.h>
#include <cg/operations/orientation.h>

#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>

namespace cg
{

// TODO: use range circulator
template <class Scalar>
bool has_intersection(const segment_2t<Scalar> & seg, const contour_2t<Scalar> & cont)
{
   for (auto cont_iter = cont.begin(); cont_iter != cont.end(); cont_iter++)
   {
      auto cont_iter_next = std::next(cont_iter);
      if (cont_iter_next == cont.end())
         cont_iter_next = cont.begin();
      segment_2t<Scalar> side(*cont_iter, *cont_iter_next);
      if (seg[0] == side[0] || seg[0] == side[1] ||
          seg[1] == side[0] || seg[1] == side[1])
         continue;
      if (has_intersection(seg, side))
         return true;
   }
   return false;
}

template <class Scalar>
bool has_intersection(const segment_2t<Scalar> & seg, const std::vector< contour_2t<Scalar> > & obstacles)
{
   return std::any_of(obstacles.begin(), obstacles.end(), [&](contour_2t<Scalar> c){ return has_intersection(seg, c); });
}

template <class Scalar>
size_t find_visible(const point_2t<Scalar> & p, typename std::vector< contour_2t<Scalar> >::const_iterator p_cont_iter,
                  const std::vector< contour_2t<Scalar> > & polygons,
                  std::back_insert_iterator< std::vector<segment_2t<Scalar>> > out_iter)
{
   size_t added = 0;
   for (auto cont_iter = polygons.begin(); cont_iter != polygons.end(); cont_iter++)
   {
      for (auto pt_iter = cont_iter->begin(); pt_iter != cont_iter->end(); pt_iter++)
      {
         if (p == *pt_iter)
            continue;

         segment_2t<Scalar> seg(p, *pt_iter);
         bool ok = !has_intersection(seg, polygons);
         if (cont_iter == p_cont_iter)
         {
            auto prev = (pt_iter == cont_iter->begin()   ? cont_iter->end() - 1 : std::prev(pt_iter));
            auto next = (pt_iter == cont_iter->end() - 1 ? cont_iter->begin()   : std::next(pt_iter));

            auto first_rotate  = orientation(*prev, *pt_iter, p);
            auto second_rotate = orientation(*pt_iter, *next, p);

            if (orientation(*prev, *pt_iter, *next) == CG_LEFT)
            {
               ok &= (first_rotate != CG_LEFT || second_rotate != CG_LEFT);
            }
            else
            {
               ok &= (first_rotate != CG_LEFT && second_rotate != CG_LEFT);
            }
         }

         if (ok && (p < *pt_iter || p_cont_iter == polygons.end()))
         {
            *out_iter++ = seg;
            added++;
         }
      }
   }
   return added;
}

template <class Scalar>
std::vector< segment_2t<Scalar> > make_visibility_graph(const point_2t<Scalar> & s, const point_2t<Scalar> & f,
                                                        const std::vector< contour_2t<Scalar> > & obstacles)
{
   std::vector< segment_2t<Scalar> > graph;

   for (auto cont_iter = obstacles.begin(); cont_iter != obstacles.end(); cont_iter++)
   {
      for (auto pt_iter = cont_iter->begin(); pt_iter != cont_iter->end(); pt_iter++)
      {
         size_t last = graph.size();
         size_t added = find_visible(*pt_iter, cont_iter, obstacles, std::back_inserter(graph));

         auto prev = (pt_iter == cont_iter->begin()   ? cont_iter->end() - 1 : std::prev(pt_iter));
         auto next = (pt_iter == cont_iter->end() - 1 ? cont_iter->begin()   : std::next(pt_iter));

         /*auto end = std::remove_if(graph.end() - added, graph.end(),
            [=](segment_2t<Scalar> s)
            {
               point_2t<Scalar> o = s[1];
               return orientation(*prev, *pt_iter, o) == CG_RIGHT && orientation(*pt_iter, *next, o) == CG_RIGHT;
            });
         graph.resize( graph.size() - std::distance(end, graph.end()) );*/

         for(; last < graph.size(); ++last)
         {
            std::cout << (graph[last][0] == *pt_iter) << std::endl;
            if (orientation(*prev, *pt_iter, graph[last][1]) == CG_RIGHT &&
                orientation(*pt_iter, *next, graph[last][1]) == CG_RIGHT)
            {
               std::cout << "yes!!!!!!!!!!!!!!!!" << std::endl;
               graph.erase(graph.begin() + last);
               --last;
            }
         }
      }
   }

   find_visible(s, obstacles.end(), obstacles, std::back_inserter(graph));
   find_visible(f, obstacles.end(), obstacles, std::back_inserter(graph));

   segment_2t<Scalar> sf(s, f);
   if(!has_intersection(sf, obstacles))
      graph.push_back(sf);

   return std::move(graph);
}


}
