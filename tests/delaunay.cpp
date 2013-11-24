#include <algorithm>
#include <vector>

#include <gtest/gtest.h>
#include <boost/assign/list_of.hpp>

#include <cg/primitives/point.h>
#include <cg/primitives/triangle.h>
#include <cg/io/triangle.h>
#include <cg/triangulation/delaunay_triangulation.h>
#include <cg/triangulation/triangulation_predicate.h>

bool check_delaunay(std::vector<cg::triangle_2> & ans)
{
   for (cg::triangle_2 tr : ans)
   {
      for (cg::triangle_2 p_tr : ans)
      {
         for (int i = 0; i < 3; i++)
         {
            cg::point_2 p = p_tr[i];
            if ( tr[0] != p && tr[1] != p && tr[2] != p &&
                 cg::in_circle(tr[0], tr[1], tr[2], p) )
               return false;
         }
      }
   }
   return true;
}

TEST(sample, delaunay)
{
   using cg::point_2;
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

TEST(sample1, delaunay)
{
   using cg::point_2;
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
