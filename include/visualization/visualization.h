#pragma once

#include "viewer.h"
#include "client_data_accumulator_impl.h"
#include "navigator_impl.h"
#include "mouse_event_handler_impl.h"

namespace visualization
{
    QWidget * create_main_window
        ( key_event_handler * viewer
        , printable * p
        , client_data_accumulator_t * client_data_accumulator
        , navigator_t * navigator
        , mouse_event_handler<QPoint> * me_handler
        );

    template<typename Scalar>
    void run_viewer(viewer_type<Scalar> * viewer, const char * title)
    {
        navigator_impl<Scalar> navigator;
        client_data_accumulator_impl<Scalar>    accumulator (viewer, &navigator);
        mouse_event_handler_impl<Scalar>        me_handler  (viewer, &navigator); 

        QWidget * wnd = create_main_window(viewer, viewer, &accumulator, &navigator, &me_handler);

        viewer->set_window(wnd);
        wnd->setWindowTitle(title);
        wnd->setMinimumSize(QSize(800, 600));
        wnd->show();
        qApp->exec(); 
    }
} 

