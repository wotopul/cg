import qbs.base 1.0

Product {
    type: "staticlibrary"
    name: "visualization"
    files: [ 
        "src/stdafx.h",
        "src/visualization/drawer_impl.cpp",
        "src/visualization/printer_impl.cpp",
        "src/visualization/main_window.cpp",
        "src/visualization/visualization.cpp",
        "src/visualization/draw_util.cpp", 
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

    Depends { name: "Qt"; submodules: ["core", "gui", "opengl"] }        

    Depends { name: "geom-primitives" }
}

