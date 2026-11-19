#include "App.hpp"

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
  ////					       GLFW INIT ////
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
  ////                         OpenGL ///
  /////////////////////////////////////////////////////////////////////////////////////////////////

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD !!\n";
    return false;
  }

  //  Resizable viewport oh yeah
  glfwSetFramebufferSizeCallback(window_,
                                 [](GLFWwindow *window, int width, int height) {
                                   glViewport(0, 0, width, height);
                                 });

  // IMGUI : INIT TODO

  return true;
}
void Albedo::ShutDown() noexcept {

  glfwDestroyWindow(window_);
  glfwTerminate();
}
void Albedo::Run() noexcept {

  running_ = true;

  float lastTime{static_cast<float>(glfwGetTime())};

  while (running_ && !glfwWindowShouldClose(window_)) {
    glfwPollEvents();

    float currentTime{static_cast<float>(glfwGetTime())};
    float dt{currentTime - lastTime};
    lastTime = currentTime;

    Update(dt);
    Render(); // what the fuck will do in this

    // OpenGL Binding and stuff

    // RENDER
    glClearColor(0.1f, 0.1f, 0.1f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Swap buffers and Poll IO events
    glfwSwapBuffers(window_);

    // IMGUI::TODO
  }
}
void Albedo::Update(float dt) noexcept {}
void Albedo::Render() noexcept {}
void Albedo::UIRender() noexcept {}
Albedo::~Albedo() { ShutDown(); }
} // namespace Rem
