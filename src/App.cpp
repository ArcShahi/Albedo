#include <App.hpp>

#include <fstream>

#include <languages/TextEditorGLSL.h>
#include <ErrorLog.hpp>


using namespace std::string_literals;


namespace Rem {

bool Albedo::Init() noexcept {

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  ////    					         GLFW 
  /////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma region GLFW

  if (!glfwInit()) {
    Rem::LogError("GLFW: Initialization failed !!");
    return false;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window_ = glfwCreateWindow(1920, 1080, "Albedo", nullptr, nullptr);

  if (!window_) {
    Rem::LogError("GLFW: Window init failed !!");  
    return false;
  }

  Rem::SetDarkTitleBar(window_, true);
  glfwMakeContextCurrent(window_);
  glfwSwapInterval(1); // vsync

#pragma endregion

  //////////////////////////////////////////////////////////////////////////////////////////////////
  ////                                     OpenGL
  /////////////////////////////////////////////////////////////////////////////////////////////////

#pragma region OpenGL

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    Rem::LogError("GLAD: Failed to initialize !!!");
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
    Rem::LogError("Texture not loaded !!!");
  }

  // Frame buffer and texture target
  int fbw{}, fbh{};
  glfwGetFramebufferSize(window_, &fbw, &fbh);
  targetTexture_ = Rem::CreateTexture(fbw, fbh);

  fbo_ = Rem::CreateFrameBufferWithTexture(targetTexture_);
  if (!fbo_.handle) {
    Rem::LogError("Framebuffer fucked");
    return false;
  }

#pragma endregion

  //////////////////////////////////////////////////////////////////////////////////////////////////
  ////                                     ImGUI
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

#pragma region EDITOR

  std::ifstream file(SHADERS + "/frag.shader"s);
  const std::string src{std::istreambuf_iterator<char>(file), {}};

  editor_.SetLanguage(TextEditorGlsl::getDefinition());
  editor_.SetPalette(TextEditor::GetDarkPalette());
  editor_.SetText(src);
  CompileFromEditor_();

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

    const float currentTime{static_cast<float>(glfwGetTime())};
    const float dt{currentTime - lastTime};
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

  ImGui::Begin("Editor");

  if (ImGui::IsKeyPressed(ImGuiKey_F5, false)) {
      CompileFromEditor_();
  }

  editor_.Render("##Fragment"); // fills the rest of the panel

  ImGui::End();

#pragma endregion

#pragma region SHADER_INPUTS_PANEL

  ImGui::Begin("Shader inputs");
  const auto &io{ImGui::GetIO()};

  ImGui::Text("%.0f FPS", io.Framerate);

  ImGui::SeparatorText("Uniforms");
  ImGui::BulletText("float      uTime        %.2f", glfwGetTime());
  ImGui::BulletText("vec2       uResolution  %d, %d", targetTexture_.width,
                    targetTexture_.height);
  ImGui::BulletText("sampler2D  uTexture     unit 0 (albedo.png)");

  ImGui::End();

#pragma endregion

#pragma region LOG_PANEL

  ImGui::Begin("LOG");

  const ImVec4 green{0.4f, 1.0f, 0.4f, 1.0f};
  const ImVec4 red{1.0f, 0.4f, 0.4f, 1.0f};

  if (shader_.Log().empty()) {
    ImGui::TextColored(green, "Compiled in %.3f ms ;)", compileTime_);
  } else {
    ImGui::TextColored(red, "Compile failed (%.2f ms) =_=", compileTime_);
    ImGui::Separator();

    ImGui::PushStyleColor(ImGuiCol_Text, red);
    ImGui::TextWrapped("[ ERROR ]: %s", shader_.Log().c_str());
    ImGui::PopStyleColor();
  }

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

void Albedo::CompileFromEditor_() noexcept {

  const auto start{glfwGetTime()};
  const bool hoe{shader_.CreateShader(editor_.GetText())};
  compileTime_ = static_cast<float>((glfwGetTime() - start) * 1000.0f);

  if (!hoe) {
    shader_.SetUniform("uTexture",0); // new shader program so have to add again
    showlog_ = false;
  } else {
    showlog_ = true;
  }
}
} // namespace Rem
