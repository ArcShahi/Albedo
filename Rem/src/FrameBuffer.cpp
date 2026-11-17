/*#include <Rem/Renderer/FrameBuffer.hpp>

#include <iostream>
#include <stb_image.h>

namespace Rem {

 FrameBuffer CreateFramebufferWithTexture(const Texture texture) {

  FrameBuffer fb;

  glCreateFramebuffers(1, &fb.handle);

  if (!AttachTextureToFramebuffer(fb, texture)) {
    glDeleteFramebuffers(1, &fb.handle);
    return {};
  }

  return fb;
}
bool AttachTextureToFramebuffer(FrameBuffer &framebuffer,
                                const Texture texture)noexcept {

  glNamedFramebufferTexture(framebuffer.handle, GL_COLOR_ATTACHMENT0,
                            texture.GetHandle(), 0);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cerr << "Framebuffer is not complete!\n";
    return false;
  }

  framebuffer.colorAttachment = texture;
  return true;
}

void BlitFramebufferToSwapchain(const FrameBuffer framebuffer) noexcept {
  glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer.handle);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // swapchain

  glBlitFramebuffer(0, 0, framebuffer.colorAttachment.GetWidth(),
                    framebuffer.colorAttachment.GetHeight(), // Source rect
                    0, 0, framebuffer.colorAttachment.GetWidth(),
                    framebuffer.colorAttachment.GetHeight(), // Destination rect
                    GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

} // namespace Rem*/