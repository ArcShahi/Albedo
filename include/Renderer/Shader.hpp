#pragma once

#include <string>
#include<unordered_map>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

namespace Rem {

class Shader {

public:
  Shader() {};

  // Active the shader
  void Use() const noexcept;

  // Check if shader object is valid
  [[nodiscard]] bool IsValid() const noexcept { return handle_ != 0; }

  // Destroy shader object
  void Destroy() noexcept;

  // Create shader program after compiling and linking shaders
  [[nodiscard]] bool CreateShader(const std::string &src) noexcept;

  // Sets any vec4 uniform
  void SetUniform(const std::string& name, glm::vec4 val) const noexcept;

  // Sets any float uniform
  void SetUniform(const std::string& name, float val) const noexcept;

  // Sets any int uniform
  void SetUniform(const std::string& name, int val) const noexcept;

  // Sets any 4x4 matrix unifrom
  void SetUniform(const std::string& name, const glm::mat4x4 &matrix) const noexcept;

  // Sets any vec2 uniform
  void SetUniform(const std::string& name, float x,float y) const noexcept;

  [[nodiscard]] const std::string &Log() const { return log_; }

private:

  [[nodiscard]] int32_t GetUniformLocation_(const char *name) const noexcept;

private:
  mutable std::unordered_map<std::string, int> uniformLocationCache_{};
  std::string log_{};
  uint32_t handle_{};

};

} // namespace Rem
