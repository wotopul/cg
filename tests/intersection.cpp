#include <cg/operations/intersection/segment_segment.h>
#include <gtest/gtest.h>
#include <vector>
#include <initializer_list>
#include <boost/variant.hpp>

TEST(intersection, simple)
{
   auto test = cg::intersection({{0, 0}, {1, 1}}, {{1, 0}, {0, 1}}, -2);
   
   ASSERT_TRUE(test == cg::segment_segment_intersection_t(cg::point_2(0.5, 0.5)));
}