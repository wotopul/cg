#include <QColor>
#include <QApplication>

#include <boost/optional.hpp>

#include "cg/visualization/interactive_viewer.h"
#include "cg/visualization/interactive/triangle.h"

#include "cg/io/point.h"

#include <cg/primitives/triangle.h>
#include <cg/primitives/point.h>

#include <cg/operations/contains/triangle_point.h>

using cg::point_2f;
using cg::point_2;
using cg::triangle_2;


struct triangle_contains_point_viewer : cg::visualization::interactive_viewer
{
   triangle_contains_point_viewer()
      : triangle_(cg::triangle_2(cg::point_2(-100.0, -100.0), cg::point_2(0.0, 100.0), cg::point_2(100.0, -100.0)))
      , point_(cg::point_2(0.0, 0.0))
   {
      interactive_.push_back(&triangle_);
      interactive_.push_back(&point_);
   }

   void print(cg::visualization::printer_type & p) const
   {

      if (cg::contains(triangle_.get_triangle(), point_.get_point()))
         p.corner_stream() << "Triangle contains point" << cg::visualization::endl;
      else
         p.corner_stream() << "Triangle DOES NOT contain point" << cg::visualization::endl;
   }
   
private:
   cg::visualization::interactive_primitive<triangle_2> triangle_;
   cg::visualization::interactive_primitive<point_2> point_;
};

int main(int argc, char ** argv)
{
   QApplication app(argc, argv);
   triangle_contains_point_viewer viewer;
   cg::visualization::run_viewer(&viewer, "triangle contains point");
}
