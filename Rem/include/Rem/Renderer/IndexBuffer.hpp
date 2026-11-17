#pragma once

#include <cstdint>
#include <glad/glad.h>

namespace Rem {

class IndexBuffer {
public:
  IndexBuffer(){}

  void Init(const uint32_t *data, uint32_t count) {
    count_ = count;
    glCreateBuffers(1, &handle_);
    glNamedBufferStorage(handle_, count_ * sizeof(uint32_t), data,
                         GL_DYNAMIC_STORAGE_BIT);
  }

  ~IndexBuffer() { glDeleteBuffers(1, &handle_); }

  [[nodiscard]] uint32_t ElementCount() const noexcept { return count_; }
  [[nodiscard]] const uint32_t &GetHandle() const noexcept { return handle_; }

private:
  uint32_t handle_{};
  uint32_t count_{};
};

} // namespace Rem