#pragma once

#include "cg/visualization/viewer.h"

#include <boost/optional.hpp>

namespace cg {
namespace visualization {

   struct interactive_type
   {
      virtual void draw (drawer_type &, bool chosen, QColor const &) const = 0;
      
      virtual float distance (point_2f const & pos)                  const = 0;
      
      virtual bool on_key     (int key)              { return false; }
      virtual bool on_move    (point_2f const & pos) { return false; }
      virtual bool on_press   (point_2f const & pos) { return false; }
      virtual bool on_release (point_2f const & pos) { return false; }
      
      virtual ~ interactive_type ( ) { }
      
      static QColor default_choose_color ( ) { return Qt::yellow; }
      static QColor default_drag_color   ( ) { return Qt::red; }
      static constexpr float default_point_size = 5.0f;
   };
   
   // Specialized for various primitives.
   template <typename Primitive>
   struct interactive_primitive;

}}
