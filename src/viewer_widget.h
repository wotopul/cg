#pragma once

#include <QGLWidget>

class QTextEdit;

#include "visualization/viewer.h"

namespace visualization
{
    struct printable;
    struct client_data_accumulator_t; 
    struct navigator_t;
    template<class Point> struct mouse_event_handler;

    struct viewer_widget_t : QGLWidget
    {
        viewer_widget_t
            ( QWidget * parent
            , QTextEdit * text_field
            , printable * viewer
            , client_data_accumulator_t * client_data_accumulator
            , navigator_t * navigator
            , mouse_event_handler<QPoint> * me_handler
            );

        void repaint();

    private:
        void initializeGL();
        void resizeGL(int, int);
        void paintGL();

        void wheelEvent(QWheelEvent *);
        void mousePressEvent(QMouseEvent *);
        void mouseDoubleClickEvent(QMouseEvent *);
        void mouseMoveEvent(QMouseEvent *);
        void mouseReleaseEvent(QMouseEvent *);

    private:
        void set_gl_transform(int screen_w, int screen_h);
        void update_text_field(bool update_user_text);
        void update_client_data();

    private:
        QTextEdit * text_field_;

        printable * viewer_;
        std::string user_text_;

        client_data_accumulator_t * client_data_accumulator_;
        navigator_t * navigator_;
        mouse_event_handler<QPoint> * mouse_event_handler_;

        optional<QPoint> start_point_;
    };
}

