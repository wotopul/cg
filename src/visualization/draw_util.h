#pragma once

#include "visualization/viewer.h"

#include <geom/primitives/rectangle.h>
#include <geom/primitives/contour.h>

namespace visualization
{
    template<class Traits>
    void draw(drawer_type<Traits> & drawer, geom::structures::rectangle_type<Traits> const & rect);

    template<class Traits>
    void draw(drawer_type<Traits> & drawer, geom::structures::contour_type<Traits>   const & cnt, bool draw_vertices = false);
}

namespace visualization
{
    template<class Traits>
    void draw(drawer_type<Traits> & drawer, geom::structures::rectangle_type<Traits> const & rect)
    {
        drawer.draw_line(rect.corner(0, 0), rect.corner(1, 0));
        drawer.draw_line(rect.corner(1, 0), rect.corner(1, 1));
        drawer.draw_line(rect.corner(1, 1), rect.corner(0, 1));
        drawer.draw_line(rect.corner(0, 1), rect.corner(0, 0));
    }

    template<class Traits>
    void draw(drawer_type<Traits> & drawer, geom::structures::contour_type<Traits>   const & cnt, bool draw_vertices = false)
    {
        geom::structures::contour_circulator<Traits> beg(cnt), it = beg;

        do
        {
            auto pt = *it;
            if (draw_vertices)
                drawer.draw_point(pt, 3);
            ++it;
            drawer.draw_line(pt, *it);
        }
        while (it != beg);
    }
}
