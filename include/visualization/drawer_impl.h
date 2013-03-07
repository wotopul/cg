#pragma once

#include "viewer.h"
#include "client_data_accumulator.h"

namespace visualization
{
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

    virtual void print(dpoint const & pos, std::string const & text) override;

    drawer_impl
            ( drect const & viewport
            , client_data_accumulator_t * acc            
            )
        : current_color_(Qt::black)
        , viewport_(viewport)
        , acc_(acc)
    {}

private:
    void draw_point(dpoint const & pt, float radius);
    void draw_line(dpoint const & a, dpoint const & b, float width);

private:
    QColor current_color_;
    drect viewport_;
    client_data_accumulator_t * acc_;
};

}

namespace visualization
{

template<class Traits>
void drawer_impl<Traits>::set_color(QColor const & c)
{
    current_color_ = c;
}

template<typename Scalar>
void drawer_impl<Scalar>::print(dpoint const & pos, std::string const & text)
{
    acc_->strings.push_back({dpoint(pos.x - viewport_.x.inf, pos.y - viewport_.y.inf), text});
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
    if  (   acc_->segment_buffers.empty() 
        ||  acc_->segment_buffers.back().width != width 
        ||  acc_->segment_buffers.back().color != current_color_
        )
    {
        acc_->segment_buffers.push_back(segment_buffer_t());
        acc_->segment_buffers.back().width = width;
        acc_->segment_buffers.back().color = current_color_;
    }
    
    std::vector<GLfloat> & buffer = acc_->segment_buffers.back().segments;

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
    if  (   acc_->point_buffers.empty() 
        ||  acc_->point_buffers.back().radius != radius 
        ||  acc_->point_buffers.back().color != current_color_
        )
    {
        acc_->point_buffers.push_back(point_buffer_t());
        acc_->point_buffers.back().radius = radius;
        acc_->point_buffers.back().color  = current_color_;
    }
    
    std::vector<GLfloat> & buffer = acc_->point_buffers.back().points;

    buffer.push_back(pt.x - viewport_.x.inf);
    buffer.push_back(pt.y - viewport_.y.inf);
}

}

