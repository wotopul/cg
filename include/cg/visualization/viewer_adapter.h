#pragma once

#include "viewer.h"

namespace cg {
namespace visualization
{
   struct viewer_adapter : viewer_type
   {
      virtual void print  (printer_type & ) const {}

      virtual void set_window(QWidget * wnd) { wnd_ = wnd; }

      virtual bool on_key_press     (int key)             { return false; }
      virtual bool on_key_release   (int key)             { return false; }
      virtual bool on_double_click  (point_2f const &)    { return false; }
      virtual bool on_mouse_move    (point_2f const &)    { return false; }
      virtual bool on_mouse_press   (point_2f const &)    { return false; }
      virtual bool on_mouse_release (point_2f const &)    { return false; }
      virtual bool on_zoom          (float)               { return false; }

   protected:
      QWidget * get_wnd() const { return wnd_; }

   private:
      QWidget * wnd_;
   };
}}
