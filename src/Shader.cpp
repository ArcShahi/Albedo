#include <Renderer/Shader.hpp>

#include <string>
#include <utility>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Rem {

namespace {


#pragma region RAW_SHADERS

// Don't even touch it
constexpr const char *VERTEX_SHADER_SRC{R"(
 #version 460 core
 out vec2 vUv;
 void main() {
  vec2 p = vec2((gl_VertexID << 1) & 2, gl_VertexID & 2);
  vUv = p;
  gl_Position = vec4(p * 2.0 - 1.0, 0.0, 1.0);
})"};


constexpr const char *FRAG_UNIFORMS{R"(
#version 460      
out vec4 FragColor; 
in  vec2 vUv;
uniform sampler2D uTexture; 
uniform float uTime;
uniform vec2 uResolution;
uniform vec2 uMouse;
)"};


#pragma endregion

[[nodiscard]] std::string InfoLog(const uint32_t &handle, bool isProgram) {

  int len{};
  if (isProgram)
    glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &len);
  else
    glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &len);

  if (len <= 1)
    return {};

  std::string info(len, '\0');
  if (isProgram)
    glGetProgramInfoLog(handle, len, nullptr, info.data());
  else
    glGetShaderInfoLog(handle, len, nullptr, info.data());

  return info;
}

[[nodiscard]] uint32_t CompileShader(uint32_t type, const char *src,
                                     std::string &log) {

  const auto handle{glCreateShader(type)};
  glShaderSource(handle, 1, &src, nullptr);
  glCompileShader(handle);

  int hoe{};
  glGetShaderiv(handle, GL_COMPILE_STATUS, &hoe);
  if (!hoe) {
    log = InfoLog(handle, false);
    glDeleteShader(handle);
    return 0;
  }
  return handle;
}

} // namespace

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

void Shader::Use() const noexcept { glUseProgram(handle_); }

void Shader::Destroy() noexcept {
  glDeleteProgram(handle_);
  handle_ = 0;

  // Don't really need to clear it ; the hasmap will fking call it
  uniformLocationCache_.clear();
}

// Create the shader program 
bool Shader::CreateShader(const std::string &fragsrc) noexcept {

  std::string log{};
  const uint32_t vs{CompileShader(GL_VERTEX_SHADER, VERTEX_SHADER_SRC, log)};
  const uint32_t fs{CompileShader(
      GL_FRAGMENT_SHADER, std::string(FRAG_UNIFORMS + fragsrc).c_str(), log)};

  if (!fs || !vs) {
    log_ = std::move(log);
    glDeleteShader(fs);
    glDeleteShader(vs);
    return false;
  }

  const uint32_t program{glCreateProgram()};
  glAttachShader(program, fs);
  glAttachShader(program, vs);
  glLinkProgram(program);
  glDetachShader(program, fs);
  glDetachShader(program, vs);
  glDeleteShader(fs);
  glDeleteShader(vs);

  int hoe{};
  glGetProgramiv(program, GL_LINK_STATUS, &hoe);
  if (!hoe) {
    log_ = InfoLog(program, true);

    glDeleteProgram(program);
    return false;
  }

  // Swap with update shader
  glDeleteProgram(handle_);
  handle_ = program;
  uniformLocationCache_.clear();
  log_.clear();
  return true;
}


void Shader::SetUniform(const std::string &name, glm::vec4 val) const noexcept {
  glUniform4f(GetUniformLocation_(name.c_str()), val.x, val.y, val.z, val.w);
}

void Shader::SetUniform(const std::string &name, float val) const noexcept {
  glUniform1f(GetUniformLocation_(name.c_str()), val);
}

void Shader::SetUniform(const std::string &name, int val) const noexcept {
  glUniform1i(GetUniformLocation_(name.c_str()), val);
}

void Shader::SetUniform(const std::string &name,
                        const glm::mat4x4 &matrix) const noexcept {
  // We don't need to transpose it if we've row major order
  glUniformMatrix4fv(GetUniformLocation_(name.c_str()), 1, false,
                     glm::value_ptr(matrix));
}

void Shader::SetUniform(const std::string &name, float x,
                        float y) const noexcept {
  glUniform2f(GetUniformLocation_(name.c_str()), x, y);
}

} // namespace Rem