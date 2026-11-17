#pragma once

#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

namespace Rem {

class Shader {

public:
  Shader() {};

  void Init(const std::string &vertexPath,
            const std::string &shaderPath) noexcept;
  void Use() const noexcept;
 
  void Reload(const char *vs, const char *fs) noexcept;
  // Sets the value of uniform vec4
  void SetUniform(const char *name, glm::vec4 val) const noexcept;
  // Sets the value of uniform float
  void SetUniform(const char *name, float val) const noexcept;

  void SetUniform(const char *name, int val) const noexcept;

  void SetUniform(const char *name, const glm::mat4x4 &matrix) const noexcept;

  [[nodiscard]] const uint32_t &GetHandle() const { return handle_; }

private:
  [[nodiscard]] uint32_t
  CreateShader_(const std::string &vertexShader,
                const std::string &fragmentShader) const noexcept;
  [[nodiscard]] uint32_t
  CompileShader_(uint32_t type, const std::string &source) const noexcept;

  [[nodiscard]] int32_t GetUniformLocation_(const char *name) const noexcept;

private:
  mutable std::unordered_map<std::string, int> uniformLocationCache_{};
  uint32_t handle_{};
};

} // namespace Rem

#endif