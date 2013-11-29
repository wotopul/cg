#pragma once

#include <cg/primitives/segment.h>
#include <cg/primitives/contour.h>
#include <cg/operations/orientation.h>

#include <vector>
#include <algorithm>
#include <iterator>

namespace cg
{

template <class Scalar>
std::vector< segment_2t<Scalar> > make_visibility_graph(const point_2t<Scalar> & s, const point_2t<Scalar> & f,
                                                      const std::vector< contour_2t<Scalar> > & obstacles)
{
   std::vector< segment_2t<Scalar> > graph;
   return std::move(graph);
}


}
