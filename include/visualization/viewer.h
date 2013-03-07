#pragma once

#include <geom/primitives/point.h>
#include <geom/primitives/segment.h>
#include <geom/primitives/line.h>

class QColor;

namespace visualization 
{
    using geom::structures::dpoint;

    template<typename Scalar>
    struct drawer_type
    {
        typedef geom::structures::point_type<Scalar>   point_t;
        typedef geom::structures::segment_type<Scalar> segment_t;
        typedef 
            typename geom::structures::line_intersection_type<Scalar>::result_type
            line_intersection_t;

        virtual void set_color  (QColor const &)                                     = 0;
        virtual void draw_line  (segment_t const &,                float width  = 1) = 0;
        virtual void draw_line  (point_t const &, point_t const &, float width  = 1) = 0;
        virtual void draw_point (point_t const &,                  float radius = 1) = 0;
        virtual void draw_point (line_intersection_t const &,      float radius = 1) = 0;

        virtual void draw_vertical_line(double x) = 0;
        virtual void print(dpoint const & pos, std::string const & text) = 0;

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

