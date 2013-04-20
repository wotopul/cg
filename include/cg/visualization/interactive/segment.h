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
      
      interactive_primitive ( )
        : chosen_(nullptr)
      { }
      
      interactive_primitive (segment_type const & segment)
         : beg_(segment[0]), end_(segment[1])
         , chosen_(nullptr)
      { }
      
      segment_type get_segment ( ) const
      {
         return segment_type(beg_.get_point(), end_.get_point());
      }
      
      void set_segment (segment_type const & segment)
      {
          beg_.set_point(segment[0]);
          end_.set_point(segment[1]);
      }
      
      virtual void draw (drawer_type & drawer, bool chosen, QColor const & color) const override
      {
         drawer.set_color(color);
         drawer.draw_line(beg_.get_point(), end_.get_point(), 1);
         beg_.draw(drawer, chosen && (chosen_ == &beg_), color);
         end_.draw(drawer, chosen && (chosen_ == &end_), color);
      }
      
      virtual float distance (point_2f const & pos) const override
      {
         float d_beg = beg_.distance(pos);
         float d_end = end_.distance(pos);
         if (d_beg < d_end)
         {
            chosen_ = const_cast<interactive_primitive<point_type> *>(&beg_);
            return d_beg;
         }
         else
         {
            chosen_ = const_cast<interactive_primitive<point_type> *>(&end_);
            return d_end;
         }
      }
      
      virtual bool on_move (point_2f const & pos) override
      {
         if (chosen_)
         {
            chosen_->on_move(pos);
            return true;
         }
         return false;
      }
      
      virtual bool on_press (point_2f const & pos) override
      {
         if (chosen_)
            chosen_->on_press(pos);
         return true;
      }
      
      virtual bool on_release (point_2f const & pos) override
      {
         if (chosen_)
            chosen_->on_release(pos);
         return true;
      }
      
   private:
   
      interactive_primitive<point_type> beg_, end_;
   
      mutable interactive_primitive<point_type> * chosen_;

   };
   
}}
