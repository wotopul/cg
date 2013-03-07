#pragma once

#include "client_data_accumulator.h"
#include "drawer_impl.h"
#include "navigator.h"

namespace visualization
{
    template<typename Scalar>
    struct client_data_accumulator_impl : client_data_accumulator_t
    {
        typedef viewer_type<Scalar> viewer_t;

        client_data_accumulator_impl
                ( viewer_t      const * viewer
                , navigator_t   const * navigator
                )
            : viewer_(viewer)
            , navigator_(navigator)
        {}

        virtual void accumulate() override
        {
            point_buffers.clear();
            segment_buffers.clear();
            strings.clear();

            drawer_impl<Scalar> drawer(navigator_->viewport(), this); 
            viewer_->draw(drawer);
        }

    private:
        viewer_t    const * viewer_;
        navigator_t const * navigator_;
    };
}

