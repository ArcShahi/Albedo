#pragma once

#include <cstdint>
#include <iostream>
#include <string>

#include <PlatformWin32.hpp> // 1
#include <glad/glad.h>       // 2
#include <GLFW/glfw3.h>      // 3

namespace Rem {

class Albedo {
public:
  Albedo()=default;
  

  bool Init() noexcept;
  void ShutDown() noexcept;
  void Run() noexcept;
  void Update(float dt) noexcept;
  void Render() noexcept;
  void UIRender() noexcept;


  ~Albedo();

private:
  GLFWwindow *window_{};
  uint32_t vao_{};
  bool running_{false};
};

} // namespace Rem
