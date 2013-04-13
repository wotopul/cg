#pragma once

#include "viewer_adapter.h"
#include "interactive/interactive.h"

#include <boost/optional.hpp>

namespace cg {
namespace visualization {

   struct interactive_viewer : viewer_adapter
   {
      interactive_viewer ( );

      void draw(cg::visualization::drawer_type & drawer) const override;
      
      bool on_press   (const point_2f & pos) override;
      bool on_release (const point_2f & pos) override;
      bool on_move    (const point_2f & pos) override;
      bool on_zoom    (float zoom)           override;

   protected:

      std::vector<cg::visualization::interactive_type *> interactive_;
      float choose_distance_;
      
      static constexpr float default_choose_distance = 100.0; 

   private:     

      boost::optional<cg::visualization::interactive_type *> chosen_;
      bool pressed_;
      
   };

}}
