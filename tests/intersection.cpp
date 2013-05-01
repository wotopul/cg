#include <cg/operations/intersection/segment_segment.h>
#include <gtest/gtest.h>
#include <vector>
#include <initializer_list>
#include <boost/variant.hpp>
using namespace cg;

TEST(intersection, simple)
{
   auto test = cg::intersection({{0, 0}, {1, 1}}, {{1, 0}, {0, 1}}, -2);
   
   ASSERT_TRUE(test == cg::segment_segment_intersection_t(cg::point_2(0.5, 0.5)));
}

TEST(intersection, singular_segments) 
{
   auto res = cg::intersection({{0, 0}, {0, 0}}, {{-1, -1}, {1, 1}}, -500);
   ASSERT_TRUE(res == segment_segment_intersection_t(point_2(0, 0)));
   
   res = cg::intersection({{-1, -1}, {1, 1}}, {{0, 0}, {0, 0}}, -500);
   ASSERT_TRUE(res == segment_segment_intersection_t(point_2(0, 0)));
   
   res = cg::intersection({{0, 0}, {0, 0}}, {{0, 0}, {0, 0}}, -500);
   ASSERT_TRUE(res == segment_segment_intersection_t(point_2(0, 0)));
}

TEST(intersection, segment)
{
   auto res = cg::intersection({{0, 0}, {1, 1}}, {{-1, -1}, {1, 1}}, -500);
   ASSERT_TRUE(res == segment_segment_intersection_t(segment_2({0, 0}, {1, 1})));
}

