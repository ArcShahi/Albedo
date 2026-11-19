cmake_minimum_required(VERSION 4.2)

project(stb LANGUAGES C)
add_library(stb)


target_sources(stb
    PRIVATE
        stb_image.cpp
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS .
        FILES
           ./stb_image.h
)

# Not restriction compiler options ... for now
