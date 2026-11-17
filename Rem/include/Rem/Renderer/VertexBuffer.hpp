#pragma once

#include <cstdint>
#include <glad/glad.h>

namespace Rem {

class VertexBuffer {
public:
  VertexBuffer(const void *data, uint32_t size) {
    glCreateBuffers(1, &handle_);
    // Copy the data into buffer
    glNamedBufferStorage(handle_, size, data, GL_DYNAMIC_STORAGE_BIT);
  }
  ~VertexBuffer() { glDeleteBuffers(1, &handle_); }

  [[nodiscard]] const uint32_t &GetHandle() const noexcept { return handle_; }

private:
  uint32_t handle_{};
};

} // namespace Rem
