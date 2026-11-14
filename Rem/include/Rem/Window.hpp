#pragma once

#include <functional>
#include <string>

#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>

#include "Event.hpp"

namespace Rem {

// We could save the last resize of window into a file and read it from there
struct WindowSpecs {
  std::string title{"Our Window"};

  using EventCallBackFunc=std::function<void(Event&)>;
  EventCallBackFunc eventCallback{};

  uint32_t width{1280};
  uint32_t height{720};
  bool vsync{true};
  bool isResizable{true};
};

class Window {
public:
  Window(const WindowSpecs &specs);
  ~Window();


  void Create() noexcept;
  void Destroy() noexcept;
  void RaiseEvent(Event &event);
  void Update()noexcept;

  [[nodiscard]] glm::vec2 GetFrameBufferSize() const noexcept;
  [[nodiscard]] glm::vec2 GetMousePosition() const noexcept;
  [[nodiscard]] GLFWwindow *GetHandle() const noexcept { return handle_; }
  [[nodiscard]] bool ShouldClose() const noexcept;


private:
  WindowSpecs specs_{};
  GLFWwindow *handle_{nullptr};
};


} // namespace Rem