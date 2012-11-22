#pragma once

#include <QtOpenGL>

#include "visualization/viewer.h"

namespace visualization
{

template<class ArithmeticTraits>
struct drawer_impl : drawer_type<ArithmeticTraits>
{
    typedef drawer_type<ArithmeticTraits>   base_t; 
    typedef typename base_t::segment_type   segment_t;
    typedef typename base_t::point_type     point_t;

    void set_color(QColor const & c)                                    override;

    void draw_line(segment_t const &,                   float width)    override;
    void draw_line(point_t const &, point_t const &,    float width)    override;
    void draw_point(point_t const & pt,                 float radius)   override;

    drawer_impl()
        : current_color_ (Qt::black)
    {}

    void clear();

    struct point_buffer_t
    {
        std::vector<GLfloat>    points;
        QColor                  color;
        float radius;
    };

    struct segment_buffer_t
    {
        std::vector<GLfloat>    segments;
        QColor                  color;
        float width;
    };

    std::vector<point_buffer_t>     point_buffers;
    std::vector<segment_buffer_t>   segment_buffers;

private:
    QColor current_color_;
};

}

namespace visualization
{

template<class Traits>
void drawer_impl<Traits>::clear()
{
    point_buffers.clear();
    segment_buffers.clear();
}

template<class Traits>
void drawer_impl<Traits>::set_color(QColor const & c)
{
    current_color_ = c;
}

template<class Traits>
void drawer_impl<Traits>::draw_line(point_t const & a, point_t const & b, float width)
{
    if (segment_buffers.empty() || (segment_buffers.back().width != width) ||
        (segment_buffers.back().color != current_color_))
    {
        segment_buffers.push_back(segment_buffer_t());
        segment_buffers.back().width = width;
        segment_buffers.back().color = current_color_;
    }
    
    std::vector<GLfloat> & buffer = segment_buffers.back().segments;
    
    buffer.push_back(Traits::to_float(a.x));
    buffer.push_back(Traits::to_float(a.y));
    buffer.push_back(Traits::to_float(b.x));
    buffer.push_back(Traits::to_float(b.y));
}

template<class Traits>
void drawer_impl<Traits>::draw_line(segment_t const & seg, float width)
{
    draw_line(seg[0], seg[1], width);
}

template<class Traits>
void drawer_impl<Traits>::draw_point(point_t const & pt, float radius)
{
    if (point_buffers.empty() || (point_buffers.back().radius != radius) ||
        (point_buffers.back().color != current_color_))
    {
        point_buffers.push_back(point_buffer_t());
        point_buffers.back().radius = radius;
        point_buffers.back().color  = current_color_;
    }
    
    std::vector<GLfloat> & buffer = point_buffers.back().points;
    buffer.push_back(Traits::to_float(pt.x));
    buffer.push_back(Traits::to_float(pt.y));
}

}
