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
                 cg::in_circle(tr[0], tr[1], tr[2], p) == cg::CG_IN )
               return false;
         }
      }
   }
   return true;
}

TEST(delaunay, sample0)
{
   cg::delaunay_triangulation<double> tr;
   std::vector<cg::triangle_2> ans;
   tr.add(cg::point_2(0, 0));
   tr.add(cg::point_2(1, 0));
   tr.add(cg::point_2(2, 0));
   tr.add(cg::point_2(3, 0));
   tr.add(cg::point_2(2, 2));
   tr.triangulate(std::back_inserter(ans));
   EXPECT_TRUE(check_delaunay(ans));
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
}

TEST(delaunay, simple_segment)
{
   cg::delaunay_triangulation<double> tr;
   std::vector<cg::triangle_2> ans;
   tr.add(cg::point_2(0, 0));
   tr.add(cg::point_2(4, 0));
   tr.add(cg::point_2(2, 0));
   tr.add(cg::point_2(3, 0));
   tr.add(cg::point_2(2, 2));
   tr.triangulate(std::back_inserter(ans));
   EXPECT_TRUE(check_delaunay(ans));
}

TEST(delaunay, segment)
{
   cg::delaunay_triangulation<double> tr;
   const double right = 10.;
   std::mt19937 generator;
   std::uniform_real_distribution<double> distribution(0, right - 1);
   tr.add(cg::point_2(0, 0));
   tr.add(cg::point_2(right, 0));

   const int num = 50;
   for (int i = 0; i < num; i++)
   {
      tr.add(cg::point_2(distribution(generator), 0));
   }
   tr.add(cg::point_2(2, 2));

   std::vector<cg::triangle_2> ans;
   tr.triangulate(std::back_inserter(ans));
   EXPECT_TRUE(check_delaunay(ans));
}

TEST(delaunay, line)
{
   cg::delaunay_triangulation<double> tr;
   const double right = 10.;
   std::mt19937 generator;
   std::uniform_real_distribution<double> distribution(0, right - 1);
   tr.add(cg::point_2(0, 0));

   const int num = 50;
   for (int i = 0; i < num; i++)
   {
      tr.add(cg::point_2(distribution(generator), 0));
   }
   tr.add(cg::point_2(2, 2));

   std::vector<cg::triangle_2> ans;
   tr.triangulate(std::back_inserter(ans));
   EXPECT_TRUE(check_delaunay(ans));
}
