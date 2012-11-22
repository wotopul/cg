#include "visualization/viewer.h"

namespace visualization
{
    template<class ArithmeticTraits>
    struct viewer_adapter : viewer_type<ArithmeticTraits>
    {
        typedef viewer_type<ArithmeticTraits>   base_t;
        typedef typename base_t::point_t        point_t;

        virtual void print(std::ostream &) const {};

        virtual void set_window(QWidget * wnd) { wnd_ = wnd; }

        virtual bool on_key         (int)                { return false; }
        virtual bool on_double_click(point_t const &)    { return false; }
        virtual bool on_move        (point_t const &)    { return false; }
        virtual bool on_press       (point_t const &)    { return false; }
        virtual bool on_release     (point_t const &)    { return false; }

    protected:
        QWidget * get_wnd() const { return wnd_; }

    private:
        QWidget * wnd_;
    };
}
