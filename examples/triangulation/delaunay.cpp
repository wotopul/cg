#include <QColor>
#include <QApplication>

#include <boost/optional.hpp>

#include <cg/triangulation/delaunay_triangulation.h>
#include <cg/visualization/viewer_adapter.h>
#include <cg/visualization/draw_util.h>


using cg::point_2f;
using cg::point_2;

struct delaunay_viewer : cg::visualization::viewer_adapter
{
   delaunay_viewer() {}

   void print(cg::visualization::printer_type & p) const
   {
       p.corner_stream() << "right click: add point" << cg::visualization::endl;
       p.corner_stream() << "double click: clear" << cg::visualization::endl;
   }

   void draw(cg::visualization::drawer_type & drawer) const
   {
       for (size_t i = 0; i + 2 < pts.size(); i += 3)
       {
           drawer.set_color(Qt::white);
           cg::triangle_2 tr(pts[i], pts[i + 1], pts[i + 2]);
           cg::visualization::draw(drawer, tr);
           drawer.set_color(Qt::green);
           cg::visualization::draw(drawer, tr.center(), tr.radius());
       }
   }

   bool on_release(const point_2f & p)
   {
      pts.push_back(p);
      return pts.size() > 2;
   }

   bool on_double_click(const point_2f & p)
   {
       pts.clear();
       return true;
   }

private:
   std::vector<point_2> pts;
};

int main(int argc, char ** argv)
{
   QApplication app(argc, argv);
   delaunay_viewer viewer;
   cg::visualization::run_viewer(&viewer, "delaunay_viewer");
}
