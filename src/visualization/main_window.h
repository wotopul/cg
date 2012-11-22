#pragma once

#include "visualization/viewer.h"

#include "drawer_impl.h"
#include "geom/primitives/rectangle.h"

using namespace visualization;

template<class ArithmeticTraits>
struct main_window_t : QGLWidget
{
public:
    typedef viewer_type<ArithmeticTraits>                   viewer_t;
    main_window_t(viewer_t * viewer);

private:
    typedef geom::structures::point_type<ArithmeticTraits>      point_t;
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
    void keyReleaseEvent(QKeyEvent *);

    void resize_impl(int, int);
    point_t screen_to_global(QPoint const & screen_pos) const;

private:
    void draw_string(int x, int y, const char * s);
    void draw_string_global(int, int, const char * s);

private:
    viewer_t * viewer_;

    rectangle_t         viewport_;
    point_t             current_pos_;
    point_t             center_;
    float               zoom_;

    optional<QPoint>    start_point_;

    drawer_t            drawer_;
};
