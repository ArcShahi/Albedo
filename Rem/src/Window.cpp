#include <cassert>
#include <iostream>

#include <Rem/InputEvents.hpp>
#include <Rem/Window.hpp>
#include <Rem/WindowEvents.hpp>

#include <glm/vec2.hpp>

namespace Rem {

Window::Window(const WindowSpecs &specs) : specs_{specs} {}
Window::~Window() { Destroy(); }

void Window::Create() noexcept {

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

  handle_ = glfwCreateWindow(specs_.width, specs_.height, specs_.title.c_str(),
                             nullptr, nullptr);

  if (!handle_) {
    std::cerr << "Failed to create GLFW window!\n";
    assert(false);
  }

  glfwMakeContextCurrent(handle_);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD !!\n";
    assert(false);
  }

  glfwSwapInterval(specs_.vsync ? 1 : 0);
  glfwSetWindowUserPointer(handle_, this);

  glfwSetWindowCloseCallback(handle_, [](GLFWwindow *hnd) {
    Window &window{*((Window *)glfwGetWindowUserPointer(hnd))};
    WindowClosedEvent event{};
    window.RaiseEvent(event);
  });

  glfwSetWindowSizeCallback(
      handle_, [](GLFWwindow *hnd, int width, int height) {
        Window &window{*((Window *)glfwGetWindowUserPointer(hnd))};

        WindowResizeEvent event((uint32_t)width, (uint32_t)height);
        window.RaiseEvent(event);
      });

  glfwSetKeyCallback(handle_, [](GLFWwindow *hnd, int key, int scancode,
                                 int action, int mods) {
    Window &window{*((Window *)glfwGetWindowUserPointer(hnd))};

    switch (action) {
    case GLFW_PRESS:
    case GLFW_REPEAT: {
      KeyPressedEvent event(key, action == GLFW_REPEAT);
      window.RaiseEvent(event);
      break;
    }
    case GLFW_RELEASE: {
      KeyReleasedEvent event(key);
      window.RaiseEvent(event);
      break;
    }
    }
  });

  /////////////////////////////////////////////////////////////////////////////////////
  //                       RAISE EVENT FOR MOUSE //
  ///////////////////////////////////////////////////////////////////////////////////
  glfwSetMouseButtonCallback(
      handle_, [](GLFWwindow *hnd, int button, int action, int mods) {
        Window &window{*((Window *)glfwGetWindowUserPointer(hnd))};

        switch (action) {
        case GLFW_PRESS: {
          MouseButtonPressedEvent event(button);
          window.RaiseEvent(event);
          break;
        }
        case GLFW_RELEASE: {
          MouseButtonReleasedEvent event(button);
          window.RaiseEvent(event);
          break;
        }
        }
      });

  glfwSetScrollCallback(
      handle_, [](GLFWwindow *hnd, double xOffset, double yOffset) {
        Window &window = *((Window *)glfwGetWindowUserPointer(hnd));

        MouseScrolledEvent event(xOffset, yOffset);
        window.RaiseEvent(event);
      });

  glfwSetCursorPosCallback(handle_, [](GLFWwindow *hnd, double x, double y) {
    Window &window{*((Window *)glfwGetWindowUserPointer(hnd))};

    MouseMovedEvent event(x, y);
    window.RaiseEvent(event);
  });
}

void Window::Destroy() noexcept {
  if (handle_) {
    glfwDestroyWindow(handle_);
  }
}

void Window::RaiseEvent(Event &event) {
  if (specs_.eventCallback)
    specs_.eventCallback(event);
}

void Window::Update() noexcept { glfwSwapBuffers(handle_); }

glm::vec2 Window::GetFrameBufferSize() const noexcept {
  int w, h;
  glfwGetFramebufferSize(handle_, &w, &h);
  return {static_cast<float>(w), static_cast<float>(h)};
}

glm::vec2 Window::GetMousePosition() const noexcept { 
     double x, y;
  glfwGetCursorPos(handle_, &x, &y);
     return {static_cast<float>(x), static_cast<float>(y)};
}

bool Window::ShouldClose() const noexcept {
  return glfwWindowShouldClose(handle_) != false;
}

} // namespace Rem
