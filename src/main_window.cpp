#include "stdafx.h"

#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>

#include "viewer_widget.h"
#include "visualization/navigator.h"

namespace visualization
{

struct main_window_t : QWidget
{
    explicit main_window_t
        ( key_event_handler * viewer
        , printable * 
        , client_data_accumulator_t * client_data_accumulator
        , navigator_t * navigator
        , mouse_event_handler<QPoint> * me_handler
        );

private:
    void virtual keyReleaseEvent(QKeyEvent * event) override;
    void virtual resizeEvent(QResizeEvent * event)  override;

private:
    key_event_handler   * viewer_;
    viewer_widget_t     * viewer_widget_;
    QTextEdit           * text_field_;

    navigator_t         * navigator_;
};

main_window_t::main_window_t
        ( key_event_handler * viewer
        , printable * p 
        , client_data_accumulator_t * client_data_accumulator
        , navigator_t * navigator
        , mouse_event_handler<QPoint> * me_handler
        )
    : viewer_(viewer)
    , navigator_(navigator)
{
    text_field_     = new QTextEdit(this);
    text_field_->setReadOnly(true);

    viewer_widget_  = new viewer_widget_t
        ( this
        , text_field_
        , p
        , client_data_accumulator
        , navigator
        , me_handler
        );

    auto layout = new QVBoxLayout(this);
    layout->addWidget(text_field_,      1 );
    layout->addWidget(viewer_widget_,   10);
    setLayout(layout);
}

void main_window_t::keyReleaseEvent(QKeyEvent * event)
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
        QApplication::clipboard()->setText(QString::fromStdString(navigator_->current_pos()));
    }
    else if (viewer_->on_key(event->key()))
    {
        viewer_widget_->repaint();
    }
    event->accept();
}

void main_window_t::resizeEvent(QResizeEvent * )
{
    viewer_widget_->repaint();
}

QWidget * create_main_window
    ( key_event_handler * viewer
    , printable * p
    , client_data_accumulator_t * client_data_accumulator
    , navigator_t * navigator
    , mouse_event_handler<QPoint> * me_handler
    )
{
    return new main_window_t(viewer, p, client_data_accumulator, navigator, me_handler);
}

} // end of namespace visualization

