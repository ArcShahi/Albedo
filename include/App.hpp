#pragma once

#include <cstdint>

#include <PlatformWin32.hpp> // 1
#include <glad/glad.h>       // 2
#include <GLFW/glfw3.h>      // 3

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <TextEditor.h>

#include <Renderer/Renderer.hpp>
#include <Renderer/Shader.hpp>

namespace {

struct EditorInfo {
  uint32_t LastUndoIndex{};
  float CompileTime{};
  float SkippedTime{-100.0};
  bool Dirty{true};
};

struct WindowInfo {

  glm::ivec2 pos{100, 100};
  glm::ivec2 size{1920, 1080};
};

} // namespace

class Albedo {
public:
  Albedo() = default;
  Albedo(const Albedo &) = delete;
  Albedo &operator=(const Albedo &) = delete;

  bool Init() noexcept;
  void ShutDown() noexcept;
  void Run() noexcept;
  void Update(float dt) noexcept;
  void Render() noexcept;
  void UIRender() noexcept;

  void ResizeTarget(int width, int height) noexcept;
  ~Albedo();

private:
  void ToggleFullscreen_() noexcept;
  void CompileFromEditor_() noexcept;
  void SaveSession_() noexcept;


private:
  TextEditor editor_{};
  Rem::Shader shader_{};
  Rem::FrameBuffer fbo_{};
  WindowInfo windowInfo{};
  Rem::Texture srcTexture_{};
  Rem::Texture targetTexture_{};
  GLFWwindow *window_{};

  glm::ivec2 viewportSize_{}; // Scene panel size inx px
  glm::vec2 mouse_{};
  EditorInfo editorInfo_{};
  uint32_t vao_{};
};
