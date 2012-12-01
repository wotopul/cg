#pragma once

#include "visualization/viewer.h"

#include "drawer_impl.h"
#include "geom/primitives/rectangle.h"

using namespace visualization;

template<class ArithmeticTraits>
struct viewer_widget_type : QGLWidget
{
    typedef viewer_type<ArithmeticTraits>                   viewer_t;
    typedef geom::structures::point_type<ArithmeticTraits>  point_t;

    viewer_widget_type(QWidget * parent, QTextEdit * text_field, viewer_t * viewer);

    point_t const & current_pos() const;

    void repaint();

private:
    typedef geom::structures::vector_type<ArithmeticTraits>     vector_t;
    typedef geom::structures::rectangle_type<ArithmeticTraits>  rectangle_t;

    typedef drawer_impl<ArithmeticTraits> drawer_t;
private:
    void initializeGL();
    void resizeGL(int, int);
    void paintGL();

    void wheelEvent(QWheelEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

    void resize_impl(int, int);
    point_t screen_to_global(QPoint const & screen_pos) const;

private:
    void update_text_field(bool update_user_text);

private:
    viewer_t * viewer_;

    QTextEdit * text_field_;

    rectangle_t         viewport_;
    point_t             current_pos_;
    point_t             center_;
    float               zoom_;

    optional<QPoint>    start_point_;

    drawer_t            drawer_;

    std::string         user_text_;
};
