#pragma once

#include <cassert>
#include <cstdint>
#include <type_traits>

#include <glad/glad.h>

namespace Rem {

class VertexArray {
public:
  VertexArray() { glCreateVertexArrays(1, &handle_); }
  ~VertexArray() { glDeleteVertexArrays(1, &handle_); }

  // Takes Buffer to read data from and stride
  void VertexBufferInfo(uint32_t binding, uint32_t vb_handle, uint32_t stride,
                        uint32_t offset = 0) const noexcept {
    glVertexArrayVertexBuffer(handle_, binding, vb_handle, offset, stride);
  }

  void IndextBufferInfo(uint32_t ib_handle) const noexcept {
    glVertexArrayElementBuffer(handle_, ib_handle);
  }

  // binds object to type target/slot
  void Bind() const noexcept { glBindVertexArray(handle_); }

  template <typename, int32_t N>
  void AddAttrib(uint32_t loc, uint32_t offset, uint32_t binding_slot = 0,
                 bool normalized = false) const noexcept;

private:
  uint32_t handle_{};
};

// Enables attribute,describes layout and links the layout
template <typename T, int32_t N>
inline void VertexArray::AddAttrib(uint32_t loc, uint32_t offset,
                                   uint32_t binding_slot,
                                   bool normalized) const noexcept {
  // Enable the attribute
  glEnableVertexArrayAttrib(handle_, loc);

  // Desribee the layout
  if constexpr (std::is_same_v<T, float>)
    glVertexArrayAttribFormat(handle_, loc, N, GL_FLOAT, normalized, offset);
  else if constexpr (std::is_same_v<T, uint8_t>)
    glVertexArrayAttribFormat(handle_, loc, N, GL_UNSIGNED_BYTE, normalized,
                              offset);
  else if constexpr (std::is_same_v<T, int32_t>)
    glVertexArrayAttribIFormat(handle_, loc, N, GL_INT, offset);
  else if constexpr (std::is_same_v<T, uint32_t>)
    glVertexArrayAttribIFormat(handle_, loc, N, GL_UNSIGNED_INT, offset);
  else
    assert(false);

  // link the buffer and attribute
  glVertexArrayAttribBinding(handle_, loc, binding_slot);
}

} // namespace Rem
