#include "App.hpp"

using namespace std::string_literals;

namespace {

namespace Color {
// TODO ERR LOG
constexpr auto CLEAR{"\033[0m"};
constexpr auto RED{"\033[101m"};
constexpr auto GREEN{"\033[102m"};
} // namespace Color
} // namespace

namespace Rem {

bool Albedo::Init() noexcept {

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////					       GLFW INIT
  /////////////////////////////////////////////////////////////////////////////////////////////////////

  if (!glfwInit()) {
    std::cerr << Color::RED << "Failed to intialize GLFW !!\n" << Color::CLEAR;
    return false;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window_ = glfwCreateWindow(1280, 720, "Albedo", nullptr, nullptr);

  if (!window_) {
    std::cerr << Color::RED << "Failed to initialized GLFW window !!\n"
              << Color::CLEAR;
    return false;
  }

  Rem::SetDarkTitleBar(window_, true);
  glfwMakeContextCurrent(window_);

  //////////////////////////////////////////////////////////////////////////////////////////////////
  ////                                OpenGL
  /////////////////////////////////////////////////////////////////////////////////////////////////

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD !!\n";
    return false;
  }

  //  Resizable viewport oh yeah
  glfwSetFramebufferSizeCallback(
      window_, [](GLFWwindow *, int w, int h) { glViewport(0, 0, w, h); });

  // Init OpenGL Object

  // Vertex Array
  glCreateVertexArrays(1, &vao_);

  // will be sampled by shader first pass
  srcTexture_ = Rem::LoadTexture(ASSETS+"/albedo.png"s);
  if (!srcTexture_.handle) {
    std::cerr << "Can't load texture";
  }

  // Frame buffer and texture target
  int fbw{}, fbh{};
  glfwGetFramebufferSize(window_, &fbw, &fbh);
  targetTexture_ = Rem::CreateTexture(fbw, fbh);

  fbo_ = Rem::CreateFrameBufferWithTexture(targetTexture_);
  if (!fbo_.handle) {
    std::cerr << "Framebuffer fucked\n";
    return false;
  }

  // Shader Stuff
  shader_.Init(SHADERS + "/vertex.shader"s, SHADERS + "/frag.shader"s);
  shader_.Use();
  shader_.SetUniform("uTexture", 0); // will be sampled foromslot 0

  // IMGUI : INIT TODO

  return true;
}

void Albedo::ShutDown() noexcept {
  Rem::DestroyFrameBuffer(fbo_);
  Rem::DestroyTexture(targetTexture_);
  Rem::DestroyTexture(srcTexture_);
  glDeleteVertexArrays(1, &vao_);

  glfwDestroyWindow(window_);
  window_ = nullptr;
  glfwTerminate();
}
void Albedo::Run() noexcept {

  // LOOP LOOP BABY , LOOP LOOP BABY
  float lastTime{static_cast<float>(glfwGetTime())};

  while (!glfwWindowShouldClose(window_)) {
    glfwPollEvents();

    float currentTime{static_cast<float>(glfwGetTime())};
    float dt{currentTime - lastTime};
    lastTime = currentTime;

    Update(dt);
    Render();
    UIRender();

    glfwSwapBuffers(window_);
  }
}
void Albedo::Update(float dt) noexcept {}

void Albedo::Render() noexcept {

  // Keep the target matching the window : TODO IMGUI PANEL
  int fbw{}, fbh{};
  glfwGetFramebufferSize(window_, &fbw, &fbh);
  if (fbw > 0 && fbh > 0 &&
      (fbw != targetTexture_.width || fbh != targetTexture_.height)) {
    ResizeTarget(fbw, fbh);
  }

  glBindFramebuffer(GL_FRAMEBUFFER, fbo_.handle);
  glViewport(0, 0, targetTexture_.width, targetTexture_.height);

  shader_.Use();
  shader_.SetUniform("uTime", static_cast<float>(glfwGetTime()));
  shader_.SetUniform("uResolution",
                     glm::vec2(targetTexture_.width, targetTexture_.height));

  glBindTextureUnit(0, srcTexture_.handle);
  glBindVertexArray(vao_);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Albedo::UIRender() noexcept {
  // TODO::IMGUI
  int fbw{}, fbh{};
  glfwGetFramebufferSize(window_, &fbw, &fbh);

  glBlitNamedFramebuffer(fbo_.handle, 0, 0, 0, targetTexture_.width,
                         targetTexture_.height, 0, 0, fbw, fbh,
                         GL_COLOR_BUFFER_BIT, GL_NEAREST);
}
void Albedo::ResizeTarget(int width, int height) noexcept {

  if (width <= 0 || height <= 0)
    return;

  Rem::DestroyFrameBuffer(fbo_);
  Rem::DestroyTexture(targetTexture_);

  targetTexture_ = Rem::CreateTexture(width, height);
  fbo_ = CreateFrameBufferWithTexture(targetTexture_);
}

Albedo::~Albedo() { ShutDown(); }
} // namespace Rem
