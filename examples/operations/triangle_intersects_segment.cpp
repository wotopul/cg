#include <QColor>
#include <QApplication>

#include <boost/optional.hpp>

#include "cg/visualization/viewer_adapter.h"
#include "cg/visualization/draw_util.h"

#include <cg/primitives/segment.h>
#include <cg/primitives/triangle.h>
#include <cg/primitives/point.h>

#include <cg/operations/has_intersection/triangle_segment.h>

using cg::point_2;
using cg::point_2f;

struct triangle_intersects_segment_viewer : cg::visualization::viewer_adapter
{
   triangle_intersects_segment_viewer()
      : s_(point_2(-100, -100), point_2(50, 50)),
        t_(point_2(-50, -50), point_2(50, -50), point_2(-50, 50)),
	     rbutton_pressed_(false)
   {}

   void draw(cg::visualization::drawer_type & drawer) const
   {
      drawer.set_color(Qt::white);
      if (cg::has_intersection(t_, s_))
         drawer.set_color(Qt::green);

      drawer.draw_line(s_[0], s_[1]);
      for (size_t l = 0, lp = 2; l != 3; lp = l++)
         drawer.draw_line(t_[lp], t_[l]);
	  
	   if (idx_)
	   {
		   drawer.set_color(rbutton_pressed_ ? Qt::red : Qt::yellow);
		   if((*idx_).first)
            drawer.draw_point(s_[(*idx_).second], 5);
         else
            drawer.draw_point(t_[(*idx_).second], 5);
	   }
   }

   void print(cg::visualization::printer_type & p) const
   {
      p.corner_stream() << "press mouse rbutton near segment vertex to move it"
                        << cg::visualization::endl
                        << "if lines are green there is intersection"
                        << cg::visualization::endl;
   }

   bool on_press(const point_2f & p)
   {
	   rbutton_pressed_ = true;
      return set_idx(p);
   }

   bool on_release(const point_2f & p)
   {
      rbutton_pressed_ = false;
      return false;
   }

   bool on_move(const point_2f & p)
   {
      if (!rbutton_pressed_)
         set_idx(p);
      if (!idx_)
         return true;

      if (rbutton_pressed_) {
         if((*idx_).first)
            s_[(*idx_).second] = p;
         else
            t_[(*idx_).second] = p;
      }
      return true;
   }

private:
   bool set_idx (const point_2f & p)
   {
      idx_.reset();
      float max_r;
      for (size_t l = 0; l != 3; ++l)
      {
         float current_r = (p.x - t_[l].x) * (p.x - t_[l].x) + (p.y - t_[l].y) * (p.y - t_[l].y);
         if ((idx_ && current_r < max_r) || (!idx_ && current_r < 100))
         {
            idx_ = std::make_pair(false, l);
            max_r = current_r;
         }
      }
      for (size_t l = 0; l != 2; ++l)
      {
         float current_r = (p.x - s_[l].x) * (p.x - s_[l].x) + (p.y - s_[l].y) * (p.y - s_[l].y);
         if ((idx_ && current_r < max_r) || (!idx_ && current_r < 100))
         {
            idx_ = std::make_pair(true, l);
            max_r = current_r;
         }
      }

      return idx_;
   }
	
   cg::segment_2 s_;
   cg::triangle_2 t_;
   boost::optional< std::pair<bool, size_t> > idx_;
   bool rbutton_pressed_;
};

int main(int argc, char ** argv)
{
   QApplication app(argc, argv);
   triangle_intersects_segment_viewer viewer;
   cg::visualization::run_viewer(&viewer, "triangle intersects segment");
}
