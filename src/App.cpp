#include "App.hpp"

#include <fstream>

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

#pragma region GLFW

  if (!glfwInit()) {
    std::cerr << Color::RED << "Failed to intialize GLFW !!\n" << Color::CLEAR;
    return false;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window_ = glfwCreateWindow(1920, 1080, "Albedo", nullptr, nullptr);

  if (!window_) {
    std::cerr << Color::RED << "Failed to initialized GLFW window !!\n"
              << Color::CLEAR;
    return false;
  }

  Rem::SetDarkTitleBar(window_, true);
  glfwMakeContextCurrent(window_);
  glfwSwapInterval(1); // vsync

#pragma endregion

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
  srcTexture_ = Rem::LoadTexture(ASSETS + "/albedo.png"s);
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

#pragma region SHADER

  std::ifstream file(SHADERS + "/frag.shader"s);
  fragSource_.assign(std::istreambuf_iterator<char>(file), {});

  if (shader_.CreateShader(fragSource_)) {
    shader_.SetUniform("uTexture", 0); // will be sampled from slot 0
  } else {
    std::cerr << shader_.Log();
  }

#pragma endregion

  //////////////////////////////////////////////////////////////////////////////////////////////////
  ////                                ImGUI
  /////////////////////////////////////////////////////////////////////////////////////////////////

#pragma region IMGUI
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io{ImGui::GetIO()};
  ImFont *font{io.Fonts->AddFontFromFileTTF(
      ASSETS "/JetBrainsMonoNerdFont-Medium.ttf", 24.0f)};
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window_, true);
  ImGui_ImplOpenGL3_Init("#version 460");

#pragma endregion

  return true;
}

void Albedo::ShutDown() noexcept {

  if (window_) {
    if (ImGui::GetCurrentContext()) {
      ImGui_ImplOpenGL3_Shutdown();
      ImGui_ImplGlfw_Shutdown();
      ImGui::DestroyContext();
    }

    Rem::DestroyFrameBuffer(fbo_);
    Rem::DestroyTexture(targetTexture_);
    Rem::DestroyTexture(srcTexture_);
    glDeleteVertexArrays(1, &vao_);

    glfwDestroyWindow(window_);
    window_ = nullptr;
    glfwTerminate();
  }
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

#ifdef BLITZ
  int fbw{}, fbh{};
  glfwGetFramebufferSize(window_, &fbw, &fbh);
  if (fbw > 0 && fbh > 0 &&
      (fbw != targetTexture_.width || fbh != targetTexture_.height)) {
    ResizeTarget(fbw, fbh);
  }
#endif

  // Keep the target matching the IMGUI PANEL

  if (viewportSize_.x > 0 && viewportSize_.y > 0 &&
      (viewportSize_.x != targetTexture_.width ||
       viewportSize_.y != targetTexture_.height)) {
    ResizeTarget(viewportSize_.x, viewportSize_.y);
  }

  glBindFramebuffer(GL_FRAMEBUFFER, fbo_.handle);
  glViewport(0, 0, targetTexture_.width, targetTexture_.height);

  if (shader_.IsValid()) {
    shader_.Use();
    shader_.SetUniform("uTime", static_cast<float>(glfwGetTime()));
    shader_.SetUniform("uResolution", targetTexture_.width,
                       targetTexture_.height);

    glBindTextureUnit(0, srcTexture_.handle);
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }
}

void Albedo::UIRender() noexcept {

  // Back to the window
  int fbw{}, fbh{};
  glfwGetFramebufferSize(window_, &fbw, &fbh);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, fbw, fbh);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

#pragma region EDITOR

  ImGui::Begin("Source");
  if (ImGui::Button("Compile") || ImGui::IsKeyPressed(ImGuiKey_F5)) {
    if (shader_.CreateShader(fragSource_)) {
      shader_.SetUniform("uTexture", 0);
    }
  }

  const float logHeight{ImGui::GetTextLineHeightWithSpacing() * 6};
  ImVec2 editorAvail{ImGui::GetContentRegionAvail()};

  // Buffer for Editor
  ImGui::InputTextMultiline("##Fragment", &fragSource_,
                            ImVec2(editorAvail.x, editorAvail.y - logHeight),
                            ImGuiInputTextFlags_AllowTabInput);

  ImGui::BeginChild("##Console");
  ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "%s",
                     shader_.Log().c_str());
  ImGui::EndChild();

  ImGui::End();
#pragma endregion

#pragma region SCENE

  // Scene: the render target, V flipped because GL textures are bottom-up
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  ImGui::Begin("Scene");
  const ImVec2 sceneAvail = ImGui::GetContentRegionAvail();
  viewportSize_ = {static_cast<int>(sceneAvail.x),
                   static_cast<int>(sceneAvail.y)};
  ImGui::Image((ImTextureID)(intptr_t)targetTexture_.handle, sceneAvail,
               ImVec2(0, 1), ImVec2(1, 0));
  ImGui::End();

#pragma endregion

  ImGui::PopStyleVar();
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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
