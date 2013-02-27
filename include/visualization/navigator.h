#pragma once

namespace visualization
{
    struct navigator_t
    {
        typedef QPoint screen_point_t;
        typedef QPoint screen_translation_t;

        virtual QPointF viewport_lb()   const = 0;
        virtual QSizeF  viewport_size() const = 0;

        virtual void set_current_pos(screen_point_t const & ) = 0;
        virtual void translate(screen_translation_t const & ) = 0;

        virtual bool zoom(float delta)                      = 0;
        virtual void set_screen_size(QSize const &)         = 0;

        virtual std::string current_pos()   const = 0;
        virtual std::string viewport()      const = 0;
    };
}

