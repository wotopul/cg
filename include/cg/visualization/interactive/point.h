#pragma once

#include "interactive.h"
#include "cg/primitives/point.h"

namespace cg {
namespace visualization {

   template <typename Scalar>
   struct interactive_primitive<point_2t<Scalar>> : interactive_type
   {
      typedef point_2t<Scalar> point_type;
      
      interactive_primitive ( )
         : drag_(false)
      { }
      
      interactive_primitive (point_type point)
         : point_(std::move(point)), drag_(false)
      { }

      point_type point ( ) const { return point_; }
      
      virtual void draw (drawer_type & drawer, bool chosen, QColor const & color) const override
      {
         if (drag_)
            drawer.set_color(default_drag_color());
         else if (chosen)
            drawer.set_color(default_choose_color());
         else
            drawer.set_color(color);
         drawer.draw_point(point_2f(point_.x, point_.y), default_point_size);
      }
      
      virtual float distance (point_2f const & pos) const override
      {
         return (pos.x - point_.x) * (pos.x - point_.x) + (pos.y - point_.y) * (pos.y - point_.y);
      }
      
      virtual bool on_move (point_2f const & pos) override
      {
         if (drag_)
         {
            point_.x = pos.x + drag_diff_.x;
            point_.y = pos.y + drag_diff_.y;
            return true;
         }
         return false;
      }
      
      virtual bool on_press   (point_2f const & pos) override
      {
         drag_ = true;
         drag_diff_.x = point_.x - pos.x;
         drag_diff_.y = point_.y - pos.y;
         return true;
      }
      
      virtual bool on_release (point_2f const & pos) override
      {
         drag_ = false;
         return true;
      }
      
   private:
      
      point_type point_;
   
      bool drag_;
      point_2f drag_diff_;
      
   };

}}
