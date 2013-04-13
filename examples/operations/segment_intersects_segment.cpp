#include <vector>
#include <stack>

#include <QColor>
#include <QApplication>

#include "cg/visualization/viewer_adapter.h"
#include "cg/visualization/draw_util.h"

#include "cg/visualization/interactive_viewer.h"
#include "cg/visualization/interactive/point.h"
#include "cg/visualization/interactive/segment.h"

#include "cg/io/point.h"

#include "cg/operations/has_intersection/segment_segment.h"

using cg::point_2f;
using cg::vector_2f;
using cg::segment_2;

struct sample_viewer : cg::visualization::interactive_viewer
{
   sample_viewer ( )
      : segment1_(segment_2(point_2f(-100.0, -100.0), point_2f(100.0, 100.0)))
      , segment2_(segment_2(point_2f(-100.0, 100.0), point_2f(100.0, -100.0)))
   {
      interactive_.push_back(&segment1_);
      interactive_.push_back(&segment2_);
   }
   
   void print(cg::visualization::printer_type & p) const
   {
      if (has_intersection(segment1_.segment, segment2_.segment))
         p.corner_stream() << "Segments do intersect" << cg::visualization::endl;
      else
         p.corner_stream() << "Segments do NOT intersect" << cg::visualization::endl;
   }
   
private:
    cg::visualization::interactive_primitive<segment_2> segment1_, segment2_;
};

int main(int argc, char ** argv)
{
   QApplication app(argc, argv);
   sample_viewer viewer;
   cg::visualization::run_viewer(&viewer, "test viewer");
}
 
