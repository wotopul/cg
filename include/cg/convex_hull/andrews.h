#pragma once

#include <boost/range/algorithm/sort.hpp>

#include <cg/primitives/point.h>
#include <cg/operations/orientation.h>
#include "graham.h"

namespace cg
{

   template<class BidItr>
   BidItr andrews(BidItr begin, BidItr end)
   {

      if (begin == end)
      {
         return begin;
      }

      std::iter_swap(begin, std::max_element(begin, end));

      BidItr second = ++begin;
      --begin;

      if (second == end)
      {
         return end;
      }

      BidItr last = --end;
      ++end;

      if (second == last)
      {
         return end;
      }

      std::iter_swap(last, std::min_element(second, end));

      BidItr upper_start = std::partition(second, last,
                                          [&begin, &last] (point_2 const& p)
      {
         return orientation(*begin, *last, p) == CG_RIGHT;
      });

//      for (auto i = begin; i != upper_start; ++i)
//      {
//         std::cout << *i << std::endl;
//      }
//      std::cout << "------\n";

      std::sort(begin, upper_start, [](point_2 const& p, point_2 const& q)
      {
         return p > q;
      });

      std::sort(upper_start, end);

//      for (auto i = begin; i != end; ++i)
//      {
//         std::cout << *i << std::endl;
//      }
//      std::cout << "------\n";


      for (auto i = begin; i != end; ++i)
      {
         std::cout << *i << std::endl;
      }
      std::cout << "------\n";

      BidItr t = contour_graham_hull(begin, end);

      for (auto i = begin; i != t; ++i)
      {
         std::cout << *i << std::endl;
      }
      std::cout << "------\n";

      return t;
   }
}
