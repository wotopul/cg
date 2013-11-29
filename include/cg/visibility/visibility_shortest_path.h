#pragma once

#include <cg/primitives/point.h>
#include <cg/operations/has_intersection/segment_segment.h>

#include <vector>
#include <algorithm>

namespace cg
{

template <class Scalar>
std::vector< point_2t<Scalar> > shortest_path(const point_2t<Scalar> & s, const point_2t<Scalar> & f,
                                              const std::vector< segment_2t<Scalar> > & edges)
{
   std::vector< point_2t<Scalar> > path;
   return std::move(path);
}

}
