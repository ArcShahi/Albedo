#include <Rem/Renderer/Shader.hpp>
#include <fstream>
#include <glad\glad.h>
#include <iostream>
#include <sstream>
#include <string>

namespace Rem {

static std::string ReadTextFile(const std::string &path) {

  std::cout << path << '\n';
  std::ifstream file{path};

  if (!file) {
    std::cerr << "File not read!!\n";
    return "";
  }

  std::stringstream ss{};
  ss << file.rdbuf();
  return ss.str();
}

void Shader::Init(const std::string &vertexPath,
                  const std::string &fragPath) noexcept {

  const std::string vs{ReadTextFile(vertexPath)};
  const std::string fs{ReadTextFile(fragPath)};
  handle_ = CreateShader_(vs, fs);
}

uint32_t Shader::CompileShader_(uint32_t type,
                                const std::string &source) const noexcept {
  uint32_t id{glCreateShader(type)};
  const char *src{source.c_str()};
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  int success{};
  glGetShaderiv(id, GL_COMPILE_STATUS, &success);
  if (!success) {
    int errlen{0};
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &errlen);
    std::string infoLog(errlen, '\0');
    glGetShaderInfoLog(id, errlen, nullptr, infoLog.data());
    std::cerr << "\nERROR: "
              << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment")
              << " shader compilation failed\n"
              << infoLog << '\n';
    glDeleteShader(id);
    return 0; // 0 is never a valid shader ID
  }
  return id;
}

int32_t Shader::GetUniformLocation_(const char *name) const noexcept {
  int32_t uniformLocation{};

  // Check in cache first
  if (const auto it{uniformLocationCache_.find(name)};
      it != uniformLocationCache_.end()) {
    uniformLocation = it->second;
  } else {
    uniformLocation = glGetUniformLocation(handle_, name);
    uniformLocationCache_[name] = uniformLocation;
  }

  return uniformLocation;
}

uint32_t
Shader::CreateShader_(const std::string &vertexShader,
                      const std::string &fragmentShader) const noexcept {
  auto vs{CompileShader_(GL_VERTEX_SHADER, vertexShader)};
  auto fs{CompileShader_(GL_FRAGMENT_SHADER, fragmentShader)};
  if (vs == 0 || fs == 0) {
    if (vs)
      glDeleteShader(vs);
    if (fs)
      glDeleteShader(fs);
    return 0;
  }

  auto handle{glCreateProgram()};
  glAttachShader(handle, vs);
  glAttachShader(handle, fs);
  glLinkProgram(handle);

  int success{};
  glGetProgramiv(handle, GL_LINK_STATUS, &success);
  glDeleteShader(vs);
  glDeleteShader(fs);

  if (!success) {
    int errlen{0};
    glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &errlen);
    std::string infoLog(errlen, '\0');
    glGetProgramInfoLog(handle, errlen, nullptr, infoLog.data());
    std::cerr << "\nERROR: Linking shader program failed\n" << infoLog << '\n';
    glDeleteProgram(handle);
    return 0;
  }
  return handle;
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