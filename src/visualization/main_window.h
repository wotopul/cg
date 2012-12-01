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
    viewer_widget_t * viewer_widget_;
    QTextEdit       * text_field_;
};

template<class Traits>
main_window_type<Traits>::main_window_type(viewer_t * viewer)
{
    text_field_     = new QTextEdit(this);
    text_field_->setReadOnly(true);

    viewer_widget_  = new viewer_widget_t(this, text_field_, viewer);

    auto layout = new QVBoxLayout(this);
    layout->addWidget(text_field_,      1 );
    layout->addWidget(viewer_widget_,   10);
    setLayout(layout);
}
