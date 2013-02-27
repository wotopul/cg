#pragma once

#include <io/point.h>
#include <io/rectangle.h>

#include <common/make.h>

namespace visualization
{
    template<class Scalar>
    struct navigator_impl : navigator_t
    {
        navigator_impl()
            : viewport_
                ( range_t(Scalar(0), Scalar(1))
                , range_t(Scalar(0), Scalar(1))
                )
            , screen_size_(1, 1)
        {}

        typedef geom::structures::point_type<Scalar>    world_point_t;

        world_point_t screen_to_world(screen_point_t const & screen_pos) const
        {
            const double zoom = (viewport_.x.sup - viewport_.x.inf).to_double() / screen_size_.width();
            vector_t screen_translation(Scalar(screen_pos.x()), Scalar(screen_size_.height() - screen_pos.y())); 
            auto res = static_cast<world_point_t>
                            ( viewport_.corner(0, 0) 
                            + zoom * screen_translation
                            );

            for (size_t axis = 0; axis <= 1; ++axis)
            {
                make_max(res[axis], viewport_[axis].inf);
                make_min(res[axis], viewport_[axis].sup);
            }

            return res;
        }

        virtual QPointF viewport_lb()   const override
        {
            return QPointF(
                viewport_.x.inf.to_double(),
                viewport_.y.inf.to_double()
            );
        }

        virtual QSizeF  viewport_size() const override
        {
            auto size = world_viewport_size();
            return QSizeF(
                size.x.to_double(),
                size.y.to_double()
            );
        }

        virtual std::string current_pos() const override
        {
            return boost::lexical_cast<std::string>(current_pos_);
        }

        virtual std::string viewport() const override
        {
            return boost::lexical_cast<std::string>(viewport_);
        }

        virtual bool zoom(float delta) override
        {
            try 
            {
                viewport_ = rectangle_t
                    ( static_cast<world_point_t>(current_pos_ + delta * (static_cast<vector_t>(viewport_.corner(0, 0) - current_pos_)))
                    , static_cast<world_point_t>(current_pos_ + delta * (static_cast<vector_t>(viewport_.corner(1, 1) - current_pos_)))
                    );
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
                auto size = world_viewport_size();
                try
                {
                    viewport_.x.sup = static_cast<Scalar>
                        ( viewport_.x.inf 
                        + Scalar::from_double(size.x.to_double() * screen_size.width() / screen_size_.width())
                        );
                    viewport_.y.sup = static_cast<Scalar>
                        ( viewport_.y.inf
                        + Scalar::from_double(size.y.to_double() * screen_size.height() / screen_size_.height())
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
            const double zoom = (viewport_.x.sup - viewport_.x.inf).to_double() / screen_size_.width();
            auto world_translation = zoom * vector_t(Scalar(-tr.x()), Scalar(tr.y()));

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

