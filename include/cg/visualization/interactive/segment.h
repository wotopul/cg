#pragma once

#include "interactive.h"
#include "point.h"
#include "cg/primitives/segment.h" 

namespace cg {
namespace visualization {

   template <typename Scalar>
   struct interactive_primitive<segment_2t<Scalar>> : interactive_type
   {

      typedef segment_2t<Scalar> segment_type;
      typedef point_2t<Scalar> point_type;
      
      segment_type segment;
      
      interactive_primitive ( )
         : drag_(false)
      { }
      
      interactive_primitive (segment_type segment)
         : segment(std::move(segment)), drag_(false)
      { }
      
      virtual void draw (drawer_type & drawer, bool chosen, QColor const & color) const override
      {
         drawer.set_color(color);
         drawer.draw_line(point_type(segment[0].x, segment[0].y), point_type(segment[1].x, segment[1].y), 1);
         draw_point(&segment[0], drawer, chosen, color);
         draw_point(&segment[1], drawer, chosen, color);
      }
      
      virtual float distance (point_2f const & pos) const override
      {
         float d_beg =  (pos.x - segment[0].x) * (pos.x - segment[0].x) + (pos.y - segment[0].y) * (pos.y - segment[0].y);
         float d_end =  (pos.x - segment[1].x) * (pos.x - segment[1].x) + (pos.y - segment[1].y) * (pos.y - segment[1].y);
         if (d_beg < d_end)
         {
            chosen_ = const_cast<point_type *>(&segment[0]);
            return d_beg;
         }
         else
         {
            chosen_ = const_cast<point_type *>(&segment[1]);
            return d_end;
         }
      }
      
      virtual bool on_move    (point_2f const & pos) override
      {
         if (drag_)
         {
            chosen_->x = pos.x + drag_diff_.x;
            chosen_->y = pos.y + drag_diff_.y;
            return true;
         }
         return false;
      }
      
      virtual bool on_press   (point_2f const & pos) override
      {
         drag_ = true;
         drag_diff_.x = chosen_->x - pos.x;
         drag_diff_.y = chosen_->y - pos.y;
         return true;
      }
      
      virtual bool on_release (point_2f const & pos) override
      {
         drag_ = false;
         return true;
      }
      
   private:
   
      mutable point_type * chosen_;
      bool drag_;
      point_2f drag_diff_;

      void draw_point (point_type const * point, drawer_type & drawer, bool chosen, QColor const & color) const
      {
         if (chosen && chosen_ == point)
            if (drag_)
               drawer.set_color(default_drag_color());
            else
               drawer.set_color(default_choose_color());
         else
            drawer.set_color(color);
         drawer.draw_point(point_2f(point->x, point->y), default_point_size);
      }

   };
   
}}
