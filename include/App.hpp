#pragma once

#include <cstdint>
#include <iostream>
#include <string>

#include <PlatformWin32.hpp> // 1
#include <glad/glad.h>       // 2
#include <GLFW/glfw3.h>      // 3


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
  Rem::FrameBuffer fbo_{};
  Rem::Texture srcTexture_{};
  Rem::Texture targetTexture_{};
  GLFWwindow *window_{};
  uint32_t vao_{};

};

} // namespace Rem
