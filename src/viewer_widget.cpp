#include "stdafx.h"

#include "viewer_widget.h"

#include "visualization/client_data_accumulator.h"
#include "visualization/navigator.h"

namespace visualization 
{

viewer_widget_t::viewer_widget_t
        ( QWidget * parent
        , QTextEdit * text_field
        , printable * viewer
        , client_data_accumulator_t * client_data_accumulator
        , navigator_t * navigator
        , mouse_event_handler<QPoint> * me_handler
        )
    : QGLWidget(parent)
    , text_field_(text_field)
    , viewer_(viewer)
    , client_data_accumulator_(client_data_accumulator)
    , navigator_(navigator)
    , mouse_event_handler_(me_handler)
{
    setMouseTracking(true);
    update_client_data();
}

void viewer_widget_t::update_client_data()
{
    client_data_accumulator_->accumulate();
    update_text_field(true);
}

void viewer_widget_t::initializeGL()
{
    assert(doubleBuffer());
    setAutoBufferSwap(true);
    qglClearColor(Qt::black);
}

void viewer_widget_t::resizeGL(int screen_w, int screen_h)
{
    navigator_->set_screen_size(QSize(screen_w, screen_h));
    set_gl_transform(screen_w, screen_h);
}

void viewer_widget_t::set_gl_transform(int screen_w, int screen_h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    QSizeF viewport_size = navigator_->viewport_size();

    glOrtho(
        0, viewport_size.width(),
        0, viewport_size.height(),
        -1.0, 1.0
        );

    glViewport(0, 0, screen_w, screen_h);
}

void viewer_widget_t::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    for (auto const & buffer : client_data_accumulator_->point_buffers)
    {
        glPointSize(buffer.radius);
        qglColor(buffer.color);

        glEnableClientState(GL_VERTEX_ARRAY);
        
        glVertexPointer(2, GL_FLOAT, 0, &buffer.points[0]);
        glDrawArrays(GL_POINTS, 0, buffer.points.size() / 2);

        glDisableClientState(GL_VERTEX_ARRAY);
    }

    for (auto const & buffer : client_data_accumulator_->segment_buffers)
    {
        glLineWidth(buffer.width);
        qglColor(buffer.color);
        
        glEnableClientState(GL_VERTEX_ARRAY);

        glVertexPointer(2, GL_FLOAT, 0, &buffer.segments[0]);

        glDrawArrays(GL_LINES, 0, buffer.segments.size() / 2);
        
        glDisableClientState(GL_VERTEX_ARRAY);
    }
}

void viewer_widget_t::wheelEvent(QWheelEvent * e)
{
    if (navigator_->zoom(std::pow(1.1f, e->delta() / 8 / 15)))
    {
        set_gl_transform(width(), height());
        repaint();
    }
}

void viewer_widget_t::mousePressEvent(QMouseEvent * e)
{
    if (e->button() == Qt::LeftButton && e->modifiers() == Qt::NoModifier)
        start_point_ = e->pos();
    else if (e->button() == Qt::RightButton)
    {
        if (mouse_event_handler_->on_press(e->pos()))
            repaint();
    }
    e->accept();
}

void viewer_widget_t::mouseMoveEvent(QMouseEvent * e)
{
    if (start_point_)
    {
        navigator_->translate(e->pos() - *start_point_);
        start_point_ = e->pos();
        navigator_->set_current_pos(e->pos());
        update_client_data();
    }
    else
    {
        navigator_->set_current_pos(e->pos());
        if (mouse_event_handler_->on_move(e->pos()))
            update_client_data();
        else
            update_text_field(false);
    }

    e->accept();
    updateGL();
}

void viewer_widget_t::mouseReleaseEvent(QMouseEvent * e)
{
    if (e->button() == Qt::LeftButton)
        start_point_ = boost::none;
    else if (mouse_event_handler_->on_release(e->pos()))
        repaint();

    e->accept();
}

void viewer_widget_t::mouseDoubleClickEvent(QMouseEvent * e)
{
    if (mouse_event_handler_->on_double_click(e->pos()))
        repaint();
}

void viewer_widget_t::update_text_field(bool update_user_text)
{
    if (update_user_text)
    {
        std::ostringstream oss;
        viewer_->print(oss);
        user_text_ = oss.str();
    }

    text_field_->clear();

    text_field_->setTextColor(Qt::blue);
    text_field_->insertPlainText("Viewport: ");

    text_field_->setTextColor(Qt::black); 
    text_field_->insertPlainText(QString::fromStdString(navigator_->viewport()));
    text_field_->insertPlainText(", ");

    text_field_->setTextColor(Qt::blue);
    text_field_->insertPlainText("mouse pos: ");

    text_field_->setTextColor(Qt::black); 
    text_field_->insertPlainText(QString::fromStdString(navigator_->current_pos()));

    text_field_->append(QString::fromStdString(user_text_));
}

void viewer_widget_t::repaint()
{
    update_client_data();
    updateGL();
}

} // end of namespace visualization

