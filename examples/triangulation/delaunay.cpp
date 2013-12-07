#include <QColor>
#include <QApplication>

#include <cmath>
#include <random>
#include <vector>
#include <algorithm>
#include <boost/optional.hpp>

#include <cg/primitives/point.h>
#include <cg/primitives/triangle.h>
#include <cg/triangulation/delaunay_triangulation.h>
#include <cg/triangulation/triangulation_predicate.h>
#include <cg/visualization/viewer_adapter.h>
#include <cg/visualization/draw_util.h>


using cg::point_2f;
using cg::point_2;

struct delaunay_viewer : cg::visualization::viewer_adapter
{
   delaunay_viewer() : distribution(0, 359) {}

   bool check_delaunay(const std::vector<cg::triangle_2> & ans) const
   {
      for (cg::triangle_2 tr : ans)
      {
         for (cg::triangle_2 p_tr : ans)
         {
            for (int i = 0; i < 3; i++)
            {
               cg::point_2 p = p_tr[i];
               if ( tr[0] != p && tr[1] != p && tr[2] != p &&
                    cg::in_circle(tr[0], tr[1], tr[2], p) == cg::CG_IN )
               {
                  // std::cerr << p << "lies inside [ " << tr[0] << " " << tr[1] << " " << tr[2] << std::endl;
                  return false;
               }
            }
         }
      }
      return true;
   }

   void print(cg::visualization::printer_type & p) const
   {
      p.corner_stream() << "right click: add point" << cg::visualization::endl;
      p.corner_stream() << (check_delaunay(answer) ? "OK" : "HE OK") << cg::visualization::endl;
   }

   void draw(cg::visualization::drawer_type & drawer) const
   {
      std::for_each(answer.begin(), answer.end(),
         [&](cg::triangle_2 tr)
         {
            drawer.set_color(Qt::white);
            cg::visualization::draw(drawer, tr);
            drawer.set_color(Qt::green);
            cg::visualization::draw(drawer, tr.center(), tr.radius());
         });
   }

   bool on_release(const point_2f & p)
   {
      pts.push_back(p);
      triangulation.add(p);
      answer.clear();
      triangulation.triangulate(std::back_inserter(answer));
      return true;
   }

   bool on_double_click(const point_2f & p)
   {
      pts.clear();
      triangulation.clear();
      return true;
   }

private:
   std::mt19937 generator;
   std::uniform_real_distribution<double> distribution;

   std::vector<point_2> pts;
   cg::delaunay_triangulation<double> triangulation;
   std::vector<cg::triangle_2> answer;
};

int main(int argc, char ** argv)
{
   QApplication app(argc, argv);
   delaunay_viewer viewer;
   cg::visualization::run_viewer(&viewer, "delaunay_viewer");
}
