#include <Rem/Renderer/Renderer.hpp>
#include <stb_image.h>

#include <cassert>

namespace Rem {

Texture::Texture(const std::string &path) : filepath_{path} {

  stbi_set_flip_vertically_on_load(
      1); // OpenGL texture coord : bottom left is 0,0 instead of top left

  // Load the image
  data_ = stbi_load(filepath_.c_str(), &width_, &height_, &bpp_, 4);

  //                    type , count, id
  glCreateTextures(GL_TEXTURE_2D, 1, &handle_);

  // Configs for our texture : must be present else blackscreen
  glTextureParameteri(handle_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTextureParameteri(handle_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTextureParameteri(handle_, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTextureParameteri(handle_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // Allocate immutable storage (1 mip level), then upload the pixels
  glTextureStorage2D(handle_, 1, GL_RGBA8, width_, height_);
  glTextureSubImage2D(handle_, 0, 0, 0, width_, height_, GL_RGBA,
                      GL_UNSIGNED_BYTE, data_);

  // Generate minimap
  glGenerateTextureMipmap(handle_);
  if (data_) {
    stbi_image_free(data_); // may wanna retain it..
    data_ = nullptr;
  }
}


Texture::~Texture() { glDeleteTextures(1, &handle_); }

void Texture::bind(uint32_t slot)const noexcept { glBindTextureUnit(slot, handle_); }

} // namespace Rem