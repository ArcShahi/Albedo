cmake_minimum_required(VERSION 4.2)

project(imgui LANGUAGES CXX)
add_library(imgui)

target_sources(imgui
    PRIVATE
        imgui.cpp
        imgui_demo.cpp
        imgui_draw.cpp
        imgui_tables.cpp
        imgui_widgets.cpp
        imgui_impl_glfw.cpp
        imgui_impl_opengl3.cpp
        imgui_stdlib.cpp

    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS .
        FILES
            imconfig.h
            imgui.h
            imgui_internal.h
            imgui_impl_glfw.h
            imgui_impl_opengl3.h
            imgui_impl_opengl3_loader.h
            imstb_rectpack.h
            imstb_textedit.h
            imstb_truetype.h
            imgui_stdlib.h
)


target_link_libraries(imgui PUBLIC glfw)

