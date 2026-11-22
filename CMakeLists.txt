cmake_minimum_required(VERSION 4.2)
project(Albedo 
    VERSION 1.1
    DESCRIPTION "Albedo : Fragment shader renderer and editor"
    LANGUAGES CXX
)

add_executable(albedo)


# Copy the assets needed to run the executables
add_custom_command(
    TARGET albedo
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:albedo>/assets"

    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/shaders"
        "$<TARGET_FILE_DIR:albedo>/shaders"

    VERBATIM
)


target_sources(albedo 
        PRIVATE 
        src/App.cpp
        src/Main.cpp
        src/Shader.cpp
        src/Renderer.cpp
    
        # Can't have this...
        PRIVATE  
          FILE_SET HEADERS 
          BASE_DIRS include 
             FILES
             include/App.hpp
             include/PlatformWin32.hpp
             include/Renderer/Shader.hpp
             include/Renderer/Renderer.hpp
             include/ErrorLog.hpp
             include/GlslTheme.hpp
)

if(WIN32)
    target_sources(albedo PRIVATE ./icon.rc)
endif()

include(dependencies.cmake)
add_subdirectory(vendors/glad)
add_subdirectory(vendors/stb)
add_subdirectory(vendors/imgui)
add_subdirectory(vendors/goossens)

target_compile_features(albedo PRIVATE cxx_std_23)

# Match VS filters to directory structure on disk
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})

# Remove console window
target_link_options(albedo PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup>
)

# MSVC FLags
target_compile_options(albedo PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/MP;/W4;/Ox;/WX;/permissive>)

# CLANG FLAGS
set(CLANG_FLAGS
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wold-style-cast
    -Wunused
    -Wconversion
    -Wsign-conversion
    -Wdouble-promotion
    -Werror)

target_compile_options(albedo PRIVATE $<$<CXX_COMPILER_ID:Clang>:${CLANG_FLAGS}>)

# Link libraries
target_link_libraries(albedo PUBLIC glm::glm glfw glad stb imgui gsns)
# liknk dwampi( what a name) for windows...
target_link_libraries(albedo PUBLIC $<$<PLATFORM_ID:Windows>:dwmapi>)


