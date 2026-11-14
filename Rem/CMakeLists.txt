cmake_minimum_required(VERSION 4.2)
project(Rem 
    VERSION 0.1
    DESCRIPTION "Rem : simple opengl framework"
    LANGUAGES CXX
)

add_library(rem)

target_sources(rem 
        PRIVATE 
        src/test.cpp

        

        # Headers TODO biatch
        PUBLIC  
          FILE_SET HEADERS 
          BASE_DIRS include 
             FILES
             include/Rem/test.hpp

 )

include(dependencies.cmake)
add_subdirectory(vendors/glad)
add_subdirectory(vendors/stb)

target_compile_features(rem PRIVATE cxx_std_23)

# Enable Multiprocessor compilation for MSVC 
target_compile_options(rem PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/MP>)
# Match VS filters to directory structure on disk
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})
# Remove console if wanted 

# Link libraries
target_link_libraries(rem PUBLIC glm::glm glfw glad stb)


