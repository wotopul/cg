#include <gtest/gtest.h>
#include <cg/operations/intersections.h>
#include <initializer_list>
#include <vector>

using namespace cg;


TEST(convex_contour_point, t1)
{
   contour_2 contour( {{0, 0}, {2, 2}, {2, 0}});
   point_2 p {1, 0.5};
   EXPECT_EQ(point_in_convex_contour(contour, p), true);
}

TEST(convex_contour_point, t2)
{
   contour_2 contour( {{0, 0}, {0, 2}, {2, 2}, {2, 0}});
   point_2 p {1, 1};
   EXPECT_EQ(point_in_convex_contour(contour, p), true);
}


TEST(convex_contour_point, t3)
{
   contour_2 contour( {{0, 0}, {0, 2}, {2, 2}, {2, 0}});
   point_2 p {-1, 1};
   EXPECT_EQ(point_in_convex_contour(contour, p), false);
}

TEST(convex_contour_point, t4)
{
   contour_2 contour( {{0, 0}, {0, 2}, {2, 4}, {4, 5}, {5, 3}, {4, 1}, {2, 0}});
   point_2 p {4, 4};
   EXPECT_EQ(point_in_convex_contour(contour, p), true);
}

TEST(convex_contour_point, t5)
{
   contour_2 contour( {{0, 0}, {0, 2}, {2, 4}, {4, 5}, {5, 3}, {4, 1}, {2, 0}});
   point_2 p {1, 3};
   EXPECT_EQ(point_in_convex_contour(contour, p), true);
}

TEST(convex_contour_point, t6)
{
   contour_2 contour( {{0, 0}, {0, 2}, {2, 4}, {4, 5}, {5, 3}, {4, 1}, {2, 0}});
   point_2 p {1, 4};
   EXPECT_EQ(point_in_convex_contour(contour, p), false);
}
