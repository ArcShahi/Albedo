#pragma once

#include <Rem/Renderer/Renderer.hpp>

namespace Rem {

struct FrameBuffer {
  Texture colorAttachment{};
  uint32_t handle{0};
};

FrameBuffer CreateFramebufferWithTexture(const Texture texture);
bool AttachTextureToFramebuffer(FrameBuffer &framebuffer,
                                const Texture texture) noexcept;
void BlitFramebufferToSwapchain(const FrameBuffer framebuffer) noexcept;

} // namespace Rem