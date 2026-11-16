#pragma once

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Rem {

class Texture {
public:
  Texture(const std::string &path);
  Texture();

  ~Texture();
  void bind(uint32_t slot = 0) const noexcept;
  [[nodiscard]] int GetWidth() const  noexcept{ return width_; }
  [[nodiscard]] int GetHeight() const noexcept{ return height_; }
  [[nodiscard]] int GetHandle() const noexcept{ return handle_; }

private:
  std::string filepath_{};
  uint8_t *data_{nullptr};
  uint32_t handle_{0};
  int width_{};
  int height_{};
  int bpp_{};
};



} // namespace Rem
