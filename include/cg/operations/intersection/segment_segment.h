#pragma once

#include <boost/variant.hpp>
#include <boost/none_t.hpp>

#include <cg/primitives/point.h>
#include <cg/primitives/segment.h>

namespace cg
{
   typedef boost::variant<boost::none_t, point_2, segment_2> segment_segment_intersection_t;

   inline segment_segment_intersection_t intersection(segment_2 const & a, segment_2 const & b, int eps_pwr);
}
