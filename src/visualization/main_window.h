#pragma once

#include "viewer_widget_impl.h"

#include <QVBoxLayout>

template<class ArithmeticTraits>
struct main_window_type : QWidget
{
    typedef viewer_type<ArithmeticTraits> viewer_t;

    explicit main_window_type(viewer_t * viewer);

private:
    typedef viewer_widget_type<ArithmeticTraits> viewer_widget_t;

private:
    void virtual keyReleaseEvent(QKeyEvent * event) override;

private:
    viewer_t        * viewer_;
    viewer_widget_t * viewer_widget_;
    QTextEdit       * text_field_;
};

template<class Traits>
main_window_type<Traits>::main_window_type(viewer_t * viewer)
    : viewer_(viewer)
{
    text_field_     = new QTextEdit(this);
    text_field_->setReadOnly(true);

    viewer_widget_  = new viewer_widget_t(this, text_field_, viewer);

    auto layout = new QVBoxLayout(this);
    layout->addWidget(text_field_,      1 );
    layout->addWidget(viewer_widget_,   10);
    setLayout(layout);
}

template<class Traits>
void main_window_type<Traits>::keyReleaseEvent(QKeyEvent * event)
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
        auto txt = boost::lexical_cast<std::string>(viewer_widget_->current_pos()); 
        QApplication::clipboard()->setText(txt.c_str());
    }
    else if (viewer_->on_key(event->key()))
    {
        viewer_widget_->repaint();
    }
    event->accept();
}

