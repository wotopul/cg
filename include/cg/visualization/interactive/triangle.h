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
      {
         set_triangle(triangle);
      }
      
      triangle_type get_triangle ( ) const
      {
         return triangle_type(p_[0].get_point(), p_[1].get_point(), p_[2].get_point());
      }
      
      void set_triangle (triangle_type const & triangle)
      {
         for (size_t i = 0; i < 3; ++i)
            p_[i].set_point(triangle[i]);
      }
      
      virtual void draw (drawer_type & drawer, bool chosen, QColor const & color) const override
      {
         drawer.set_color(color);
         for (size_t i = 0; i < 3; ++i)
            drawer.draw_line(point_type(p_[i].get_point().x, p_[i].get_point().y), point_type(p_[(i + 1) % 3].get_point().x, p_[(i + 1) % 3].get_point().y), 1);
         for (size_t i = 0; i < 3; ++i)
            p_[i].draw(drawer, chosen && (chosen_ == &p_[i]), color);
      }
      
      virtual float distance (point_2f const & pos) const override
      {
         float d1 = p_[0].distance(pos);
         float d2 = p_[1].distance(pos);
         float d3 = p_[2].distance(pos);
         if (d1 < d2 && d1 < d3)
         {
            chosen_ = const_cast<interactive_primitive<point_type> *>(&p_[0]);
            return d1;
         }
         else if (d2 < d3)
         {
            chosen_ = const_cast<interactive_primitive<point_type> *>(&p_[1]);
            return d2;
         }
         else
         {
            chosen_ = const_cast<interactive_primitive<point_type> *>(&p_[2]);
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
   
      interactive_primitive<point_type> p_[3];
   
      mutable interactive_primitive<point_type> * chosen_;

   };
   
}}
 
