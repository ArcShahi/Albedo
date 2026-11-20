cmake_minimum_required(VERSION 4.2)

project(gossens LANGUAGES CXX)
add_library(gsns)

target_sources(gsns
    PRIVATE
        TextEditor.cpp
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS .
        FILES
            languages/TextEditorC.h
            languages/TextEditorGlsl.h
            TextEditor.h
            TextEditorLanguages.h
)

target_compile_features(gsns PUBLIC cxx_std_17) 
target_link_libraries(gsns PUBLIC imgui)

