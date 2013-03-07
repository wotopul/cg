#pragma once

#include "viewer.h"
#include "client_data_accumulator.h"

namespace visualization
{
    using geom::structures::dpoint;
    using geom::structures::drect;

template<typename Scalar>
struct drawer_impl : drawer_type<Scalar>
{
    typedef drawer_type<Scalar>   base_t; 
    typedef typename base_t::segment_t              segment_t;
    typedef typename base_t::point_t                point_t;
    typedef typename base_t::line_intersection_t    line_isect_t;

    typedef client_data_accumulator_t::point_buffer_t    point_buffer_t;
    typedef client_data_accumulator_t::segment_buffer_t  segment_buffer_t;

    virtual void set_color(QColor const & c)                                    override;

    virtual void draw_line(segment_t const &,                   float width)    override;
    virtual void draw_line(point_t const &, point_t const &,    float width)    override;
    virtual void draw_point(point_t const & pt,                 float radius)   override;
    virtual void draw_point(line_isect_t const &,               float radius)   override;

    virtual void draw_vertical_line(double x) override;

    drawer_impl
            ( drect const & viewport
            , std::vector<point_buffer_t>     & point_buffers
            , std::vector<segment_buffer_t>   & segment_buffers
            )
        : current_color_(Qt::black)
        , viewport_(viewport)
        , point_buffers(point_buffers)
        , segment_buffers(segment_buffers)
    {}

private:
    void draw_point(dpoint const & pt, float radius);
    void draw_line(dpoint const & a, dpoint const & b, float width);

private:
    QColor current_color_;
    drect viewport_;
    std::vector<point_buffer_t>     & point_buffers;
    std::vector<segment_buffer_t>   & segment_buffers;
};

}

namespace visualization
{

template<class Traits>
void drawer_impl<Traits>::set_color(QColor const & c)
{
    current_color_ = c;
}

template<class Traits>
void drawer_impl<Traits>::draw_line(point_t const & a, point_t const & b, float width)
{
    draw_line(static_cast<dpoint>(a), static_cast<dpoint>(b), width); 
}

template<class Traits>
void drawer_impl<Traits>::draw_line(segment_t const & seg, float width)
{
    draw_line(seg[0], seg[1], width);
}

template<typename Scalar>
void drawer_impl<Scalar>::draw_vertical_line(double x)
{
    draw_line(dpoint(x, viewport_.y.inf), dpoint(x, viewport_.y.sup), 1.f);
}

template<typename Scalar>
void drawer_impl<Scalar>::draw_line(dpoint const & a, dpoint const & b, float width)
{
    if (segment_buffers.empty() || (segment_buffers.back().width != width) ||
        (segment_buffers.back().color != current_color_))
    {
        segment_buffers.push_back(segment_buffer_t());
        segment_buffers.back().width = width;
        segment_buffers.back().color = current_color_;
    }
    
    std::vector<GLfloat> & buffer = segment_buffers.back().segments;

    dpoint lb = viewport_.corner(0, 0);
    auto da = a - lb;
    auto db = b - lb;
    
    buffer.push_back(da.x);
    buffer.push_back(da.y);
    buffer.push_back(db.x);
    buffer.push_back(db.y);
}

template<typename Scalar>
void drawer_impl<Scalar>::draw_point(point_t const & pt, float radius)
{
    draw_point(static_cast<dpoint>(pt), radius);
}

template<typename Scalar>
void drawer_impl<Scalar>::draw_point(line_isect_t const & ipt, float radius)
{
    const double x = static_cast<double>(ipt.x);
    const double y = static_cast<double>(ipt.y);
    const double w = static_cast<double>(ipt.w);

    draw_point(dpoint(x / w, y / w), radius);
}

template<typename Scalar>
void drawer_impl<Scalar>::draw_point(dpoint const & pt, float radius)
{
    if (point_buffers.empty() || (point_buffers.back().radius != radius) ||
        (point_buffers.back().color != current_color_))
    {
        point_buffers.push_back(point_buffer_t());
        point_buffers.back().radius = radius;
        point_buffers.back().color  = current_color_;
    }
    
    std::vector<GLfloat> & buffer = point_buffers.back().points;

    buffer.push_back(pt.x - viewport_.x.inf);
    buffer.push_back(pt.y - viewport_.y.inf);
}

}

