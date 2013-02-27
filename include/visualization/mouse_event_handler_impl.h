#pragma once

#include "viewer.h"

namespace visualization
{
    template<class Scalar>
    struct mouse_event_handler_impl : mouse_event_handler<QPoint>
    {
        mouse_event_handler_impl
                ( viewer_type<Scalar> * viewer
                , navigator_impl<Scalar> const * navigator
                )
            : viewer_(viewer)
            , navigator_(navigator)
        {}

#define proxy_method(method_name) \
    virtual bool method_name(QPoint const & pos) override \
    { \
        return viewer_->method_name(navigator_->screen_to_world(pos)); \
    }

        proxy_method(on_double_click)
        proxy_method(on_move)
        proxy_method(on_press)
        proxy_method(on_release)

#undef proxy_method

    private:
        viewer_type<Scalar> * viewer_;
        navigator_impl<Scalar> const * navigator_;
    };
}

