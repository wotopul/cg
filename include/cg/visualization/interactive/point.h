#pragma once

#include "interactive.h"
#include "cg/primitives/point.h"

namespace cg {
namespace visualization {

   template <typename Scalar>
   struct interactive_primitive<point_2t<Scalar>> : interactive_type
   {
      typedef point_2t<Scalar> point_type;
      
      point_type point;
      
      interactive_primitive ( )
         : drag_(false)
      { }
      
      interactive_primitive (point_type point)
         : point(std::move(point)), drag_(false)
      { }
      
      virtual void draw (drawer_type & drawer, bool chosen, QColor const & color) const override
      {
         if (drag_)
            drawer.set_color(default_drag_color());
         else if (chosen)
            drawer.set_color(default_choose_color());
         else
            drawer.set_color(color);
         drawer.draw_point(point_2f(point.x, point.y), default_point_size);
      }
      
      virtual float distance (point_2f const & pos) const override
      {
         return (pos.x - point.x) * (pos.x - point.x) + (pos.y - point.y) * (pos.y - point.y);
      }
      
      virtual bool on_move    (point_2f const & pos) override
      {
         if (drag_)
         {
            point.x = pos.x + drag_diff_.x;
            point.y = pos.y + drag_diff_.y;
            return true;
         }
         return false;
      }
      
      virtual bool on_press   (point_2f const & pos) override
      {
         drag_ = true;
         drag_diff_.x = point.x - pos.x;
         drag_diff_.y = point.y - pos.y;
         return true;
      }
      
      virtual bool on_release (point_2f const & pos) override
      {
         drag_ = false;
         return true;
      }
      
   private:
   
      bool drag_;
      point_2f drag_diff_;
      
   };

}}
