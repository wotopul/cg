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

    struct key_event_handler
    {
        virtual bool on_key(int key) = 0; 
    };

    template<class Point>
    struct mouse_event_handler
    {
        virtual bool on_double_click(Point const & pos) = 0;
        virtual bool on_move        (Point const & pos) = 0;
        virtual bool on_press       (Point const & pos) = 0;
        virtual bool on_release     (Point const & pos) = 0;
    };

    struct printable
    {
        virtual void print(std::ostream &) const = 0;
    };

    template<typename Scalar>
    struct viewer_type 
        : key_event_handler
        , mouse_event_handler<geom::structures::point_type<Scalar>>
        , printable
    {
        typedef drawer_type<Scalar> drawer_t;

        virtual void draw   (drawer_t &  ) const    = 0;
        virtual void set_window(QWidget *)          = 0;

        virtual ~viewer_type() {};
    };

    template<class ArithmeticTraits>
    void run_viewer(viewer_type<ArithmeticTraits> * viewer, const char * title);

} // namespace visualization

