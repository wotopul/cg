#pragma once

#include <io/point.h>

#include <common/make.h>

namespace visualization
{
    using geom::structures::dvec;

    template<class Scalar>
    struct navigator_impl : navigator_t
    {
        navigator_impl()
            : screen_size_(1, 1)
        {
            typedef typename Scalar::value_type sv_t;

            for (size_t axis = 0; axis != 2; ++axis)
            {
                viewport_[axis] = range_t(Scalar(sv_t(0)), Scalar(sv_t(1)));
            }
        }

        typedef geom::structures::point_type<Scalar>    world_point_t;

        world_point_t screen_to_world(screen_point_t const & screen_pos) const
        {
            const double zoom = static_cast<double>(viewport_.x.sup - viewport_.x.inf) / screen_size_.width();
            dvec screen_translation(screen_pos.x(), screen_size_.height() - screen_pos.y()); 
            world_point_t res(static_cast<dpoint>(viewport_.corner(0, 0)) + zoom * screen_translation);

            for (size_t axis = 0; axis <= 1; ++axis)
            {
                make_max(res[axis], viewport_[axis].inf);
                make_min(res[axis], viewport_[axis].sup);
            }

            return res;
        }

        virtual drect viewport() const override
        {
            return static_cast<drect>(viewport_);
        }

        virtual std::string current_pos() const override
        {
            return boost::lexical_cast<std::string>(current_pos_);
        }

        virtual bool zoom(float delta) override
        {
            dpoint p = static_cast<dpoint>(current_pos_);
            try 
            {
                world_point_t corner[2];
                for (size_t i = 0; i != 2; ++i)
                {
                    corner[i] = static_cast<world_point_t>
                        ( p 
                        + delta * (static_cast<dvec>(viewport_.corner(i, i) - current_pos_))
                        );
                }
                viewport_ = rectangle_t(corner[0], corner[1]);
            } 
            catch (arithmetic::overflow_error const &)
            {
                return false;
            }

            return true;
        }

        virtual void set_current_pos(screen_point_t const & screen_pos) override
        {
            current_pos_ = screen_to_world(screen_pos);
        }

        virtual void set_screen_size(QSize const & screen_size) override
        {
            for (;;)
            {
                auto size = static_cast<dvec>(world_viewport_size());
                try
                {
                    viewport_.x.sup = static_cast<Scalar>
                        ( viewport_.x.inf 
                        + static_cast<Scalar>(size.x * screen_size.width() / screen_size_.width())
                        );
                    viewport_.y.sup = static_cast<Scalar>
                        ( viewport_.y.inf
                        + static_cast<Scalar>(size.y * screen_size.height() / screen_size_.height())
                        );
                    break;
                }
                catch (arithmetic::overflow_error const &)
                {
                    zoom(0.9);
                }
            }

            screen_size_ = screen_size;
            current_pos_  = viewport_.corner(0, 0);
        }

        virtual void translate(screen_translation_t const & tr) override
        {
            const double zoom = 
                static_cast<double>(viewport_.x.sup - viewport_.x.inf) / screen_size_.width();

            auto world_translation = 
                static_cast<vector_t>(zoom * geom::structures::dvec(-tr.x(), tr.y()));

            try
            {
                viewport_ = rectangle_t
                    ( static_cast<world_point_t>(viewport_.corner(0, 0) + world_translation)
                    , static_cast<world_point_t>(viewport_.corner(1, 1) + world_translation)
                    );
            }
            catch (arithmetic::overflow_error const &)
            {
            }
        }

    private:
        decltype(std::declval<world_point_t>() - std::declval<world_point_t>()) world_viewport_size() const
        {
            return viewport_.corner(1, 1) - viewport_.corner(0, 0);  
        }

    private:
        typedef geom::structures::rectangle_type<Scalar>    rectangle_t;
        typedef geom::structures::range_type<Scalar>        range_t;
        typedef geom::structures::vector_type<Scalar>       vector_t;

    private:
        rectangle_t     viewport_;
        QSize           screen_size_;
        world_point_t   current_pos_;
    };
}

