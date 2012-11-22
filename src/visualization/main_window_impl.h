#include "stdafx.h"

#include "main_window.h"

#include "io/point.h"
#include "geom/primitives/rectangle.h"

#include "drawer_impl.h"

template<class Traits>
main_window_t<Traits>::main_window_t(viewer_t * viewer)
    : viewer_(viewer)
    , zoom_(1.0)
{
    setMouseTracking(true);
    viewer_->draw(drawer_);
}

template<class Traits>
void main_window_t<Traits>::initializeGL()
{
    assert(doubleBuffer());
    setAutoBufferSwap(true);
    qglClearColor(Qt::black);
}

template<class Traits>
void main_window_t<Traits>::resize_impl(int screen_w, int screen_h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    const float size_x = screen_w * zoom_ / 2;
    const float size_y = screen_h * zoom_ / 2;

    glOrtho(center_.x - size_x, center_.x + size_x,
            center_.y - size_y, center_.y + size_y,
            -1.0, 1.0);
    glViewport(0, 0, screen_w, screen_h);
}

template<class Traits>
void main_window_t<Traits>::resizeGL(int screen_w, int screen_h)
{
    resize_impl(screen_w, screen_h);
}

template<class Traits>
void main_window_t<Traits>::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    for (auto const & buffer : drawer_.point_buffers)
    {
        glPointSize(buffer.radius);
        qglColor(buffer.color);

        glEnableClientState(GL_VERTEX_ARRAY);
        
        glVertexPointer(2, GL_FLOAT, 0, &buffer.points[0]);
        glDrawArrays(GL_POINTS, 0, buffer.points.size() / 2);

        glDisableClientState(GL_VERTEX_ARRAY);
    }

    for (auto const & buffer : drawer_.segment_buffers)
    {
        glLineWidth(buffer.width);
        qglColor(buffer.color);
        
        glEnableClientState(GL_VERTEX_ARRAY);

        glVertexPointer(2, GL_FLOAT, 0, &buffer.segments[0]);

        glDrawArrays(GL_LINES, 0, buffer.segments.size() / 2);
        
        glDisableClientState(GL_VERTEX_ARRAY);
    }

    //printer.corner_stream() << "Mouse pos: " << current_pos_ << endl;
    //viewer_->print(printer);
}

template<class Traits>
void main_window_t<Traits>::wheelEvent(QWheelEvent * e)
{
    float new_zoom = zoom_ * std::pow(1.1f, e->delta() / 8 / 15);

    float diff_x =  (e->pos().x() - size().width()  / 2) * (zoom_ - new_zoom);
    float diff_y = -(e->pos().y() - size().height() / 2) * (zoom_ - new_zoom); 

    auto real_line = geom::structures::range_type<Traits>::maximal(); 

    float size_x = width() * new_zoom / 2;
    float size_y = height() * new_zoom / 2;

    if (real_line.contains(center_.x + diff_x + size_x) && 
        real_line.contains(center_.x + diff_x - size_x) &&
        real_line.contains(center_.y + diff_y + size_y) && 
        real_line.contains(center_.y + diff_y - size_y))
    {
        zoom_      = new_zoom;
        center_.x += diff_x;
        center_.y += diff_y;

        viewer_->on_move(screen_to_global(e->pos()));

        resize_impl(size().width(), size().height());
        updateGL();
    }
}

template<class Traits>
void main_window_t<Traits>::mousePressEvent(QMouseEvent * e)
{
    if (e->button() == Qt::LeftButton && e->modifiers() == Qt::NoModifier)
        start_point_ = e->pos();
    else if (e->button() == Qt::RightButton)
    {
        if (viewer_->on_press(current_pos_))
        {
            drawer_.clear();
            viewer_->draw(drawer_);
            updateGL();
        }
    }
    e->accept();
}

template<class Traits>
void main_window_t<Traits>::mouseMoveEvent(QMouseEvent * e)
{
    current_pos_ = screen_to_global(e->pos());

    if (start_point_ )
    {
        const int w = size().width(); 
        const int h = size().height();

        using geom::structures::vector_type;

        const int diff_x = -(e->x() - start_point_->x()) ;
        const int diff_y =   e->y() - start_point_->y()  ;

        auto real_line = geom::structures::range_type<Traits>::maximal(); 

        if (real_line.contains(center_.x + (diff_x + w / 2) * zoom_) && 
            real_line.contains(center_.x + (diff_x - w / 2) * zoom_) &&
            real_line.contains(center_.y + (diff_y + h / 2) * zoom_) && 
            real_line.contains(center_.y + (diff_y - h / 2) * zoom_))
        {
            center_.x += diff_x * zoom_;
            center_.y += diff_y * zoom_;

            start_point_ = e->pos();

            resize_impl(w, h);
        }
    }
    else if (viewer_->on_move(current_pos_))
    {
        drawer_.clear();
        viewer_->draw(drawer_);
    }
    e->accept();
    updateGL();
}

template<class Traits>
void main_window_t<Traits>::mouseReleaseEvent(QMouseEvent * e)
{
    if (e->button() == Qt::LeftButton)
        start_point_ = boost::none;
    else if (viewer_->on_release(screen_to_global(e->pos())))
    {
        drawer_.clear();
        viewer_->draw(drawer_);
        updateGL();
    }
    e->accept();
}

template<class Traits>
void main_window_t<Traits>::mouseDoubleClickEvent(QMouseEvent * event)
{
    if (viewer_->on_double_click(screen_to_global(event->pos())))
    {
        drawer_.clear();
        viewer_->draw(drawer_);
        updateGL();
    }
}

template<class Traits>
void main_window_t<Traits>::keyReleaseEvent(QKeyEvent * event)
{
    if ((event->key() == Qt::Key_C) && (event->modifiers() == Qt::ControlModifier))
    {
        //std::stringstream ss;
        //ss << QInputDialog::getText(this, "center selection", "type point: ").toStdString();
        //point_t old_pos = current_pos_;
        //ss >> current_pos_;

        //center_ += (current_pos_ - old_pos);

        //resize_impl(size().width(), size().height());
        //updateGL();
    }
    else if ((event->key() == Qt::Key_I) && (event->modifiers() == Qt::ControlModifier))
    {
        auto txt = boost::lexical_cast<std::string>(current_pos_); 
        QApplication::clipboard()->setText(txt.c_str());
    }
    else if (viewer_->on_key(event->key()))
    {
        drawer_.clear();
        viewer_->draw(drawer_);
        updateGL();
    }
    event->accept();
}

template<class Traits>
typename main_window_t<Traits>::point_t 
    main_window_t<Traits>::screen_to_global(QPoint const & screen_pos) const
{
    return point_t (
        center_.x + zoom_ * (screen_pos.x() - width() / 2),
        center_.y + zoom_ * (height() / 2 - screen_pos.y())
    );
}
