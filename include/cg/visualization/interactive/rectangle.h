#pragma once

#include "interactive.h"
#include "point.h"
#include "cg/primitives/rectangle.h" 

namespace cg {
namespace visualization {

   template <typename Scalar>
   struct interactive_primitive<rectangle_2t<Scalar>> : interactive_type
   {
      typedef Scalar scalar_type;
      typedef rectangle_2t<Scalar> rectangle_type;
      typedef range_t<Scalar> range_type;
      typedef point_2t<Scalar> point_type;
      
      interactive_primitive ( )
      { }
      
      interactive_primitive (rectangle_type const & rectangle)
      {
         set_rectangle(rectangle);
      }
      
      rectangle_type get_rectangle ( ) const
      {
         return rectangle_type(
            range_type(p_[0][0].get_point().x, p_[1][0].get_point().x),
            range_type(p_[0][0].get_point().y, p_[0][1].get_point().y)
        );
      }
      
      void set_rectangle (rectangle_type const & rectangle)
      {
         for (size_t i = 0; i < 2; ++i)
            for (size_t j = 0; j < 2; ++j)
               p_[i][j] = rectangle.corner(i, j);
      }
      
      virtual void draw (drawer_type & drawer, bool chosen, QColor const & color) const override
      {
         drawer.set_color(color);
         for (size_t x = 0; x < 2; ++x)
            drawer.draw_line(p_[x][0].get_point(), p_[x][1].get_point(), 1);
         for (size_t y = 0; y < 2; ++y)
            drawer.draw_line(p_[0][y].get_point(), p_[1][y].get_point(), 1);
         for (size_t x = 0; x < 2; ++x)
            for (size_t y = 0; y < 2; ++y)
               p_[x][y].draw(drawer, chosen && (chosen_x_ == x && chosen_y_ == y), color);
      }
      
      virtual float distance (point_2f const & pos) const override
      {
         chosen_x_ = 0;
         chosen_y_ = 0;
         float d[2][2];
         for (size_t x = 0; x < 2; ++x)
            for (size_t y = 0; y < 2; ++y)
            {
               d[x][y] = p_[x][y].distance(pos);
               if (d[x][y] < d[chosen_x_][chosen_y_])
               {
                  chosen_x_ = x;
                  chosen_y_ = y;
               }
            }
         return d[chosen_x_][chosen_y_];
      }
      
      virtual bool on_move (point_2f const & pos) override
      {
         p_[chosen_x_][chosen_y_].on_move(pos);
         update_rectangle();
         return true;
      }
      
      virtual bool on_press (point_2f const & pos) override
      {
         p_[chosen_x_][chosen_y_].on_press(pos);
         return true;
      }
      
      virtual bool on_release (point_2f const & pos) override
      {
         p_[chosen_x_][chosen_y_].on_release(pos);
         return true;
      }
      
   private:
   
      void update_rectangle ( )
      {
         scalar_type x = p_[chosen_x_][chosen_y_].get_point().x;
         scalar_type y = p_[chosen_x_][chosen_y_].get_point().y;
         p_[chosen_x_][1 - chosen_y_].set_point(point_type(x, p_[chosen_x_][1 - chosen_y_].get_point().y));
         p_[1 - chosen_x_][chosen_y_].set_point(point_type(p_[1 - chosen_x_][chosen_y_].get_point().x, y));
      }
   
      interactive_primitive<point_type> p_[2][2];
   
      mutable size_t chosen_x_, chosen_y_;

   };
   
}}
 
