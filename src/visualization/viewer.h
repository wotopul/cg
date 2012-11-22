#pragma once

#include "geom/primitives/point.h"
#include "geom/primitives/segment.h"

class QColor;

namespace visualization 
{
    template<class ArithmeticTraits>
    struct drawer_type
    {
        typedef geom::structures::point_type<ArithmeticTraits>      point_type;
        typedef geom::structures::segment_type<ArithmeticTraits>    segment_type;

        virtual void set_color  (QColor const &)                                           = 0;
        virtual void draw_line  (segment_type const &,                   float width  = 1) = 0;
        virtual void draw_line  (point_type const &, point_type const &, float width  = 1) = 0;
        virtual void draw_point (point_type const &,                     float radius = 1) = 0;

        virtual ~drawer_type() {};
    };

    template<class ArithmeticTraits>
    struct viewer_type
    {
        typedef drawer_type<ArithmeticTraits>                   drawer_t;
        typedef geom::structures::point_type<ArithmeticTraits>  point_t;

        virtual void draw   (drawer_t &  )              const   = 0;
        virtual void print  (std::ostream & )           const   = 0;

        virtual void set_window(QWidget *)                      = 0;

        virtual bool on_key         (int key)                   = 0; 
        virtual bool on_double_click(point_t const & pos)       = 0;
        virtual bool on_move        (point_t const & pos)       = 0;
        virtual bool on_press       (point_t const & pos)       = 0;
        virtual bool on_release     (point_t const & pos)       = 0;

        virtual ~viewer_type() {};
    };

    template<class ArithmeticTraits>
    void run_viewer(viewer_type<ArithmeticTraits> * viewer, const char * title);

} // namespace visualization
