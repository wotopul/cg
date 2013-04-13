#include "cg/visualization/interactive_viewer.h"

namespace cg {
namespace visualization {

   interactive_viewer::interactive_viewer ( )
      : choose_distance_(default_choose_distance)
      , pressed_(false)
   { }
   
   void interactive_viewer::draw(cg::visualization::drawer_type & drawer) const
   {
      for (auto const i_ptr : interactive_)
         i_ptr->draw(drawer, (chosen_ && i_ptr == *chosen_), Qt::white);
   }

   bool interactive_viewer::on_press(const point_2f & pos)
   {
      pressed_ = true;
      if (chosen_)
      {
         (*chosen_)->on_press(pos);
         return true;
      }
      return false;
   }

   bool interactive_viewer::on_release(const point_2f & pos)
   {
      pressed_ = false;
      if (chosen_)
      {
         (*chosen_)->on_release(pos);
         return true;
      }
      return false;
   }

   bool interactive_viewer::on_move(const point_2f & pos)
   {
      if (!pressed_)
      {
         chosen_.reset();
         float min_distance = 0.0;
         for (auto i_ptr : interactive_)
         {
            float current_distance = i_ptr->distance(pos);
            if (current_distance < choose_distance_)
               if (!chosen_ || current_distance < min_distance)
               {
                  chosen_ = i_ptr;
                  min_distance = current_distance;
               }
         }
      }
      if (chosen_)
      {
         (*chosen_)->on_move(pos);
      }
      return true;
   }
   
   bool interactive_viewer::on_zoom (float zoom)
   {
      choose_distance_ = default_choose_distance * zoom * zoom;
      return true;
   }

}}
