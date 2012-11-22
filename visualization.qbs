import qbs.base 1.0

StaticLibrary {
    name: "visualization"
    files: [ 
        "src/stdafx.h",
        "src/visualization/drawer_impl.h",
        "src/visualization/main_window.h",
        "src/visualization/main_window_impl.h",
        "src/visualization/visualization.h",
        "src/visualization/draw_util.h", 
    ]

    Depends { name: "cpp" }
    cpp.cppFlags: "-std=c++0x"
    cpp.includePaths: [
        "src",
        "include",
        "../common/include",
        "../arithmetic/include",
        "../geom-primitives/include",
    ]
    cpp.precompiledHeader: "src/stdafx.h"

    Depends { name: "Qt"; submodules: ["core", "gui", "opengl"] }        

    Depends { name: "geom-primitives" }
}

