#include "stdafx.h"

#include "visualization/viewer.h"
#include "main_window.h"
#include "main_window_impl.h"

namespace visualization
{
    template<class ArithmeticTraits>
    void run_viewer(viewer_type<ArithmeticTraits> * viewer, const char * title)
    {
        main_window_t<ArithmeticTraits> * wnd = new main_window_t<ArithmeticTraits>(viewer);
        viewer->set_window(wnd);
        wnd->setWindowTitle(title);
        wnd->show();
        qApp->exec(); 
    }
} 
