#include <algorithm>
#include <vector>
#include <random>
#include <cmath>

#include <gtest/gtest.h>
#include <boost/assign/list_of.hpp>

#include <cg/primitives/point.h>
#include <cg/primitives/triangle.h>
#include <cg/io/triangle.h>
#include <cg/triangulation/delaunay_triangulation.h>
#include <cg/triangulation/triangulation_predicate.h>
#include <misc/random_utils.h>

#include "random_utils.h"

using cg::point_2;
using cg::point_2f;
using cg::triangle_2;

bool check_delaunay(const std::vector<cg::triangle_2> & ans)
{
   for (triangle_2 tr : ans)
   {
      for (triangle_2 p_tr : ans)
      {
         for (int i = 0; i < 3; i++)
         {
            point_2 p = p_tr[i];
            if ( tr[0] != p && tr[1] != p && tr[2] != p &&
                 cg::in_circle(tr[0], tr[1], tr[2], p) )
               return false;
         }
      }
   }
   return true;
}

TEST(delaunay, sample)
{
   std::vector<point_2> pts = boost::assign::list_of(point_2(1, 0))
                                                    (point_2(3, 3))
                                                    (point_2(5, -1))
                                                    (point_2(7, 4))
                                                    (point_2(8, 1));
   cg::delaunay_triangulation<double> tr;
   for (auto p : pts)
   {
      tr.add(p);
      std::vector<cg::triangle_2> ans;
      tr.triangulate(std::back_inserter(ans));
      EXPECT_TRUE(check_delaunay(ans));
   }
}

TEST(delaunay, sample1)
{
   std::vector<point_2> pts = boost::assign::list_of(point_2(0, 0))
                                                    (point_2(2, 3))
                                                    (point_2(2, 1))
                                                    (point_2(4, 0));
   cg::delaunay_triangulation<double> tr;
   for (auto p : pts)
   {
      tr.add(p);
      std::vector<cg::triangle_2> ans;
      tr.triangulate(std::back_inserter(ans));
      EXPECT_TRUE(check_delaunay(ans));
   }
}

TEST(delaunay, circle)
{
   std::mt19937 generator;
   std::uniform_real_distribution<double> distribution(0, 359);

   cg::delaunay_triangulation<double> tr;
   std::vector<cg::triangle_2> ans;
   const int num = 40;
   const double r = 100.;
   for (int i = 0; i < num; i++)
   {
      double alpha = distribution(generator);
      point_2 p(r * cos(alpha), r * sin(alpha));
      tr.add(p);
      ans.clear();
      tr.triangulate(std::back_inserter(ans));
      EXPECT_TRUE(check_delaunay(ans));
   }
   EXPECT_TRUE(check_delaunay(ans));
}

/*TEST(delaunay, line)
{
   uniform_random_real<double, std::mt19937> distr(-(1LL << 53), (1LL << 53));

   std::vector<cg::point_2> pts = uniform_points(1000);
   for (size_t l = 0, ln = 1; ln < pts.size(); l = ln++)
   {
      cg::point_2 a = pts[l];
      cg::point_2 b = pts[ln];

      for (size_t k = 0; k != 300; ++k)
      {
         double t = distr();
         cg::point_2 c = a + t * (b - a);
         EXPECT_EQ(cg::orientation(a, b, c), *cg::orientation_r()(a, b, c));
      }
   }
}*/
