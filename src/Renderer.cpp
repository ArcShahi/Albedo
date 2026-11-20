#include <Renderer/Renderer.hpp>
#include <ErrorLog.hpp>

#include <stb_image.h>



namespace Rem {

[[nodiscard]] Texture CreateTexture(int width, int height) noexcept {

  Texture out{};
  out.width = width;
  out.height = height;

  glCreateTextures(GL_TEXTURE_2D, 1, &out.handle);
  glTextureStorage2D(out.handle, 1, GL_RGBA8, width, height);

  glTextureParameteri(out.handle, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTextureParameteri(out.handle, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTextureParameteri(out.handle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTextureParameteri(out.handle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  return out;
}

[[nodiscard]] Texture LoadTexture(const std::string &path) noexcept {

  int width{}, height{};

  // In OpeGL texture's (0,0) is at bottom left instead of top left
  stbi_set_flip_vertically_on_load(1);
  uint8_t *data{stbi_load(path.c_str(), &width, &height, nullptr, 4)};

  if (!data) {
    Rem::LogError("Failed to load texture");
    Rem::LogError(stbi_failure_reason());
    return {};
  }

  Texture out{};
  out.width = width;
  out.height = height;

  // Full mip chain- floor(log2(max(w, h))) + 1 levels
  const auto levels{
      std::bit_width(static_cast<unsigned>(std::max(width, height)))};

  glCreateTextures(GL_TEXTURE_2D, 1, &out.handle);
  glTextureStorage2D(out.handle, levels, GL_RGBA8, width, height);
  glTextureSubImage2D(out.handle, 0, 0, 0, width, height, GL_RGBA,
                      GL_UNSIGNED_BYTE, data);
  glGenerateTextureMipmap(out.handle);

  glTextureParameteri(out.handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTextureParameteri(out.handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTextureParameteri(out.handle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTextureParameteri(out.handle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  stbi_image_free(data);

  return out;
}

void DestroyTexture(Texture &t) noexcept {
  if (t.handle)
    glDeleteTextures(1, &t.handle);
  t = {};
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
////                                    FRAMEBUFFER ////
////////////////////////////////////////////////////////////////////////////////////////////////////////

[[nodiscard]]
bool AttachTextureToFrameBuffer(FrameBuffer &framebuffer,
                                const Texture texture) noexcept {

  if (!framebuffer.handle || !texture.handle) {
    Rem::LogError("Invalid framebuffer or texture");
    return false;
  }

  glNamedFramebufferTexture(framebuffer.handle, GL_COLOR_ATTACHMENT0,
                            texture.handle, 0);

  if (glCheckNamedFramebufferStatus(framebuffer.handle, GL_FRAMEBUFFER) !=
      GL_FRAMEBUFFER_COMPLETE) {
    Rem::LogError("Framebuffer is not complete !!!");
    return false;
  }

  framebuffer.colorAttachment = texture;
  return true;
}

[[nodiscard]]
FrameBuffer CreateFrameBufferWithTexture(const Texture texture) noexcept {

  FrameBuffer out{};
  glCreateFramebuffers(1, &out.handle);

  if (!AttachTextureToFrameBuffer(out, texture)) {
    glDeleteFramebuffers(1, &out.handle);
    return {};
  }
  return out;
}

void DestroyFrameBuffer(FrameBuffer &framebuff) noexcept {
  if (framebuff.handle)
    glDeleteFramebuffers(1, &framebuff.handle);
  framebuff = {};
}

} // namespace Rem