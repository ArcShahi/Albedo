#pragma once

#include <string>

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

namespace Rem {

class Shader {

public:
  Shader() {};
  void Use() const noexcept;

  bool IsValid() const noexcept { return handle_ != 0; }
  void Destroy() noexcept;

  [[nodiscard]] bool CreateShader(const std::string &fragsrc) noexcept;

  // Sets the value of uniform vec4
  void SetUniform(const std::string& name, glm::vec4 val) const noexcept;
  // Sets the value of uniform float
  void SetUniform(const std::string& name, float val) const noexcept;

  void SetUniform(const std::string& name, int val) const noexcept;

  void SetUniform(const std::string& name, const glm::mat4x4 &matrix) const noexcept;
  void SetUniform(const std::string& name, float x,float y) const noexcept;

  [[nodiscard]] const std::string &Log() const { return log_; }

private:
  [[nodiscard]] int32_t GetUniformLocation_(const char *name) const noexcept;

private:
  mutable std::unordered_map<std::string, int> uniformLocationCache_{};
  std::string log_{};
  uint32_t handle_{};
  // TODO: ERRORLOG for Imgui Console panel
};

} // namespace Rem
