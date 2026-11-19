#pragma once

#include <cstdint>
#include <iostream>
#include <string>

#include <PlatformWin32.hpp> // 1
#include <glad/glad.h>       // 2
#include <GLFW/glfw3.h>      // 3

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_stdlib.h>


#include <Renderer/Shader.hpp>
#include <Renderer/Renderer.hpp>


namespace Rem {

class Albedo {
public:
  Albedo()=default;
  Albedo(const Albedo &) = delete;
  Albedo &operator=(const Albedo &) = delete;
  

  bool Init() noexcept;
  void ShutDown() noexcept;
  void Run() noexcept;
  void Update(float dt) noexcept;
  void Render() noexcept;
  void UIRender() noexcept;

  void ResizeTarget(int width,int height) noexcept;


  ~Albedo();

private:
  Rem::Shader shader_{};
  std::string fragSource_{};
  Rem::FrameBuffer fbo_{};
  Rem::Texture srcTexture_{};
  Rem::Texture targetTexture_{};
  GLFWwindow *window_{};

  glm::ivec2 viewportSize_{};// Screen panel size

  uint32_t vao_{};

};

} // namespace Rem
