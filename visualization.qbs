import qbs.base 1.0

StaticLibrary {
    name: "visualization"
    files: [ 
        "src/stdafx.h",
        "src/viewer_widget.h",
        "src/viewer_widget.cpp",
        "src/main_window.cpp",
        "include/visualization/client_data_accumulator.h",
        "include/visualization/client_data_accumulator_impl.h",
        "include/visualization/navigator.h",
        "include/visualization/navigator_impl.h",
        "include/visualization/visualization.h",
        "include/visualization/drawer_impl.h",
        "include/visualization/draw_util.h", 
        "include/visualization/viewer.h",
        "include/visualization/viewer_adapter.h",
    ]

    Depends { name: "cpp" }
    cpp.cppFlags: "-std=c++0x"
    cpp.includePaths: [
        "include",
        "../core/common/include",
        "../core/arithmetic/include",
        "../core/primitives/include",
        "../logger/include",
    ]
    cpp.precompiledHeader: "src/stdafx.h"

    Depends { name: "Qt"; submodules: ["core", "gui", "opengl"] }        

    Depends { name: "primitives" }
}

