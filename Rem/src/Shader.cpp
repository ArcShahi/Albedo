#include <Rem/Renderer/Shader.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <glad\glad.h>

namespace Rem {

Shader::Shader(const char *vertexPath, const char *shaderPath)
    : handle_{
          CreateShader_(ReadTextFile(vertexPath), ReadTextFile(vertexPath))} {}

uint32_t Shader::CompileShader_(uint32_t type,
                                const std::string &source) const noexcept {
  uint32_t id{glCreateShader(type)};
  const char *src{source.c_str()};

  // give OpenGL source
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  int success{};
  glGetShaderiv(id, GL_COMPILE_STATUS, &success);

  if (!success) {
    int errlen{0};
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &success);
    std::string infoLog(errlen, '0');
    glGetProgramInfoLog(id, errlen, nullptr, infoLog.data());
    std::cerr << "\nERROR: Linking shader program failed\n" << infoLog << '\n';
    return -1;
  }
  return id;
}

uint32_t
Shader::CreateShader_(const std::string &vertexShader,
                      const std::string &fragmentShader) const noexcept {
  auto handle{glCreateProgram()};
  auto vs{CompileShader_(GL_VERTEX_SHADER, vertexShader)};
  auto fs{CompileShader_(GL_FRAGMENT_SHADER, fragmentShader)};

  glAttachShader(handle, vs);
  glAttachShader(handle, fs);

  glLinkProgram(handle);
  glValidateProgram(handle);

  int success{};
  glGetProgramiv(handle, GL_LINK_STATUS, &success);
  if (!success) {
    int errlen{0};
    glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &errlen);
    std::string infoLog(errlen, '0');
    glGetProgramInfoLog(handle, errlen, nullptr, infoLog.data());
    std::cerr << "\nERROR: Linking shader program failed\n" << infoLog << '\n';
    return -1;
  }

  glDeleteShader(vs);
  glDeleteShader(fs);
  return handle;
}

std::string ReadTextFile(const char *path) {

  std::ifstream file{path};

  if (!file) {
    std::cerr << "File not read!!\n";
    return "";
  }

  std::stringstream ss{};
  ss << file.rdbuf();
  return ss.str();
}

void Shader::Use() const noexcept { glUseProgram(handle_); }

void Shader::Reload(const char *vs, const char *fs) noexcept {
  auto handle{CreateShader_(ReadTextFile(vs), ReadTextFile(fs))};
  // if failed keep the olderone
  if (handle != -1) {
    glDeleteProgram(handle_);
    handle_ = handle;
  }
}

void Shader::SetUniform(const char *name, glm::vec4 val) const noexcept {
  glUniform4f(GetUniformLocation_(name), val.x, val.y, val.z, val.w);
}

void Shader::SetUniform(const char *name, float val) const noexcept {
  glUniform1f(GetUniformLocation_(name), val);
}

void Shader::SetUniform(const char *name, int val) const noexcept {
  glUniform1i(GetUniformLocation_(name), val);
}

void Shader::SetUniform(const char *name,
                        const glm::mat4x4 &matrix) const noexcept {
  // We don't need to transpose it if we've row major order
  glUniformMatrix4fv(GetUniformLocation_(name), 1, false, &matrix[0][0]);
}

} // namespace Rem