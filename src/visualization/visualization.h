#include "stdafx.h"

#include "visualization/viewer.h"
#include "main_window.h"

namespace visualization
{
    template<class ArithmeticTraits>
    void run_viewer(viewer_type<ArithmeticTraits> * viewer, const char * title)
    {
        main_window_type<ArithmeticTraits> wnd(viewer);
        viewer->set_window(&wnd);
        wnd.setWindowTitle(title);
        wnd.setMinimumSize(QSize(800, 600));
        wnd.show();
        qApp->exec(); 
    }
} 
