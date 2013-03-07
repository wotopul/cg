#pragma once

#include "visualization/viewer.h"

namespace visualization
{
    template<typename Scalar>
    struct viewer_adapter : viewer_type<Scalar>
    {
        typedef geom::structures::point_type<Scalar> point_t;

        virtual void print(std::ostream &) const override {}

        virtual void set_window(QWidget * wnd) override { wnd_ = wnd; }

        virtual bool on_key         (int)             override { return false; }
        virtual bool on_double_click(point_t const &) override { return false; }
        virtual bool on_move        (point_t const &) override { return false; }
        virtual bool on_press       (point_t const &) override { return false; }
        virtual bool on_release     (point_t const &) override { return false; }

    protected:
        QWidget * get_wnd() const { return wnd_; }

    private:
        QWidget * wnd_;
    };
}
