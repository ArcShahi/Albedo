#pragma once

#ifdef _WIN32

#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

// Credit: https://discourse.glfw.org/t/dark-theme-titlebar/2537

// If glad was included first, it already defined APIENTRY; drop it so 
// windows.h can redefine it without C4005 (/WX).
#ifdef APIENTRY
#undef APIENTRY
#endif
#include <dwmapi.h>
#include <windows.h>

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>


#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#endif

namespace Rem {
inline void SetDarkTitleBar(GLFWwindow *window, bool enabled = true) {

  HWND hwnd{glfwGetWin32Window(window)};
  BOOL value{enabled ? TRUE : FALSE};
  ::DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &value,
                          sizeof(value));
}
} // namespace Rem
#else

struct GLFWwindow;

namespace Rem {
inline void SetDarkTitleBar(GLFWwindow *, bool = true) {}
}


#endif