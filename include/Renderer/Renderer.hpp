#pragma once

#include <cstdint>
#include <string>

#include <glad/glad.h> // First
#include <GLFW/glfw3.h>

namespace Rem {

struct Texture {
  uint32_t handle{}, width{}, height{};
};

struct FrameBuffer {
  Texture colorAttachment{};
  uint32_t handle{};
};

[[nodiscard]] Texture CreateTexture(int width, int height) noexcept;
[[nodiscard]] Texture LoadTexture(const std::string &path) noexcept;
void DestroyTexture(Texture &t) noexcept;

[[nodiscard]] FrameBuffer
CreateFrameBufferWithTexture(const Texture texture) noexcept;
[[nodiscard]] bool AttachTextureToFrameBuffer(FrameBuffer &framebuffer,
                                              const Texture texture) noexcept;

void DestroyFrameBuffer(FrameBuffer &framebuff) noexcept;
} // namespace Rem