#pragma once

#include "interactive.h"
#include "point.h"
#include "cg/primitives/triangle.h" 

namespace cg {
namespace visualization {

   template <typename Scalar>
   struct interactive_primitive<triangle_2t<Scalar>> : interactive_type
   {

      typedef triangle_2t<Scalar> triangle_type;
      typedef point_2t<Scalar> point_type;
      
      interactive_primitive ( )
      { }
      
      interactive_primitive (triangle_type const & triangle)
         : p1_(triangle[0]), p2_(triangle[1]), p3_(triangle[2])
      { }
      
      triangle_type triangle ( ) const
      {
         return triangle_type(p1_.point(), p2_.point(), p3_.point());
      }
      
      virtual void draw (drawer_type & drawer, bool chosen, QColor const & color) const override
      {
         drawer.set_color(color);
         drawer.draw_line(point_type(p1_.point().x, p1_.point().y), point_type(p2_.point().x, p2_.point().y), 1);
         drawer.draw_line(point_type(p2_.point().x, p2_.point().y), point_type(p3_.point().x, p3_.point().y), 1);
         drawer.draw_line(point_type(p3_.point().x, p3_.point().y), point_type(p1_.point().x, p1_.point().y), 1);
         p1_.draw(drawer, chosen && (chosen_ && chosen_ == &p1_), color);
         p2_.draw(drawer, chosen && (chosen_ && chosen_ == &p2_), color);
         p3_.draw(drawer, chosen && (chosen_ && chosen_ == &p3_), color);
      }
      
      virtual float distance (point_2f const & pos) const override
      {
         float d1 = p1_.distance(pos);
         float d2 = p2_.distance(pos);
         float d3 = p3_.distance(pos);
         if (d1 < d2 && d1 < d3)
         {
            chosen_ = const_cast<interactive_primitive<point_type> *>(&p1_);
            return d1;
         }
         else if (d2 < d3)
         {
            chosen_ = const_cast<interactive_primitive<point_type> *>(&p2_);
            return d2;
         }
         else
         {
            chosen_ = const_cast<interactive_primitive<point_type> *>(&p3_);
            return d3;
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
   
      interactive_primitive<point_type> p1_, p2_, p3_;
   
      mutable interactive_primitive<point_type> * chosen_;

   };
   
}}
 
