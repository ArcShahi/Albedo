#include <App.hpp>

#include <fstream>
#include <format>
#include <chrono>
#include <filesystem>
#include <system_error>

#include <ErrorLog.hpp>
#include <languages/TextEditorGLSL.h>


using namespace std::string_literals;

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
  [[maybe_unused]] ImFont *font{io.Fonts->AddFontFromFileTTF(
      ASSETS "/JetBrainsMonoNerdFont-Medium.ttf", 24.0f)};
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window_, true);
  ImGui_ImplOpenGL3_Init("#version 460");

#pragma endregion

#pragma region EDITOR

  const std::string shaderPath{SHADERS + "/frag.shader"s};
  std::ifstream file(SHADERS + "/frag.shader"s);
  if (!file) {
    Rem::LogError("Can't Open"s + shaderPath);
  }

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
    shader_.Destroy();

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

    if (glfwGetWindowAttrib(window_, GLFW_ICONIFIED)) {
      glfwWaitEvents();
      continue;
    }
    const float currentTime{static_cast<float>(glfwGetTime())};
    const float dt{currentTime - lastTime};
    lastTime = currentTime;

    Update(dt);
    Render();
    UIRender();

    glfwSwapBuffers(window_);
  }
  SaveSession_();
}

void Albedo::Update([[maybe_unused]] float dt) noexcept {
  if (ImGui::IsKeyPressed(ImGuiKey_F11, false))
    ToggleFullscreen_();
}

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
      (viewportSize_.x != static_cast<int>(targetTexture_.width) ||
       viewportSize_.y != static_cast<int>(targetTexture_.height))) {
    ResizeTarget(viewportSize_.x, viewportSize_.y);
  }

  glBindFramebuffer(GL_FRAMEBUFFER, fbo_.handle);
  glViewport(0, 0, targetTexture_.width, targetTexture_.height);

  if (shader_.IsValid()) {
    shader_.Use();
    shader_.SetUniform("uTime", static_cast<float>(glfwGetTime()));
    shader_.SetUniform("uResolution", static_cast<float>(targetTexture_.width),
                       static_cast<float>(targetTexture_.height));
    shader_.SetUniform("uMouse", mouse_.x, mouse_.y);

    glBindTextureUnit(0, srcTexture_.handle);
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, 3);
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
  const ImVec4 green{0.4f, 1.0f, 0.4f, 1.0f};
  const ImVec4 red{1.0f, 0.4f, 0.4f, 1.0f};

#pragma region EDITOR_PANEL

  ImGui::Begin("Editor");

  if (const auto undo{editor_.GetUndoIndex()};
      undo != editorInfo_.LastUndoIndex) {
    editorInfo_.LastUndoIndex = static_cast<uint32_t>(undo);
    editorInfo_.Dirty = true;
  }

  if (ImGui::IsKeyPressed(ImGuiKey_F5, false)) {
    CompileFromEditor_();
  }

  const float footerY{ImGui::GetFrameHeightWithSpacing()};
  ImGui::BeginChild("##editorArea", ImVec2(0, -footerY), ImGuiChildFlags_None,
                    ImGuiWindowFlags_NoScrollbar |
                        ImGuiWindowFlags_NoScrollWithMouse);

  editor_.Render("##Fragment"); // fills the rest of the panel
  ImGui::EndChild();

  ImGui::Separator();
  ImGui::Separator();
  if (glfwGetTime() - editorInfo_.SkippedTime < 2.0)
    ImGui::TextDisabled("No changes since last compile");
  else if (shader_.Log().empty())
    ImGui::TextColored(green, "Compiled in %.2f ms", editorInfo_.CompileTime);
  else
    ImGui::TextColored(red, "Compile failed (see log)");

  ImGui::End();
#pragma endregion

#pragma region LOG_PANEL

  ImGui::Begin("LOG");

  if (shader_.Log().empty()) {
    ImGui::TextDisabled("No errors");
  } else {
    ImGui::PushStyleColor(ImGuiCol_Text, red);
    ImGui::TextWrapped("%s", shader_.Log().c_str());
    ImGui::PopStyleColor();
  }
  ImGui::End();

#pragma endregion

#pragma region SCENE

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  ImGui::Begin("Image");

  const float sfooterY{ImGui::GetFrameHeightWithSpacing()};
  ImVec2 sceneAvail{ImGui::GetContentRegionAvail()};
  sceneAvail.y = std::max(sceneAvail.y - sfooterY, 1.0f);

  // Render target follwos the image area not whole windows
  viewportSize_ = {static_cast<int>(sceneAvail.x),
                   static_cast<int>(sceneAvail.y)};

  ImDrawList *dl{ImGui::GetWindowDrawList()};
  dl->AddCallback(
      [](const ImDrawList *, const ImDrawCmd *) {
        glDisable(GL_BLEND); // ignore alpha: draw the image as opaque
      },
      nullptr);

  // Scene: the render target, V flipped because GL textures are bottom-up
  ImGui::Image((ImTextureID)(intptr_t)targetTexture_.handle, sceneAvail,
               ImVec2(0, 1), ImVec2(1, 0));

  dl->AddCallback(ImDrawCallback_ResetRenderState,
                  nullptr); // restore ImGui's state

  if (ImGui::IsItemHovered()) {
    const ImVec2 min{ImGui::GetItemRectMin()};
    const ImVec2 size{ImGui::GetItemRectSize()};
    const ImVec2 m{ImGui::GetMousePos()};

    mouse_ = {(m.x - min.x) / size.x * targetTexture_.width,
              (1.0f - (m.y - min.y) / size.y) * targetTexture_.height};
  }

  // Footer (indented because this window has zero padding)
  ImGui::Indent(8.0f);
  ImGui::Text("%d x %d   |   %.0f FPS   |   %.2f s", targetTexture_.width,
              targetTexture_.height, ImGui::GetIO().Framerate, glfwGetTime());
  ImGui::Unindent(8.0f);

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

void Albedo::ToggleFullscreen_() noexcept {

  if (glfwGetWindowMonitor(window_)) {
    glfwSetWindowMonitor(window_, nullptr, windowInfo.pos.x, windowInfo.pos.y,
                         windowInfo.size.x, windowInfo.size.y, 0);
  } else {
    glfwGetWindowPos(window_, &windowInfo.pos.x, &windowInfo.pos.y);
    glfwGetWindowSize(window_, &windowInfo.size.x, &windowInfo.size.y);

    GLFWmonitor *monitor{glfwGetPrimaryMonitor()};
    const GLFWvidmode *mode{glfwGetVideoMode(monitor)};
    glfwSetWindowMonitor(window_, monitor, 0, 0, mode->width, mode->height,
                         mode->refreshRate);
  }
  // My fucking driver drop vsync ...
  glfwSwapInterval(1);
}

void Albedo::CompileFromEditor_() noexcept {

  if (!editorInfo_.Dirty) {
    editorInfo_.SkippedTime = static_cast<float>(glfwGetTime());
    return;
  }
  editorInfo_.Dirty = false;

  const auto start{glfwGetTime()};
  const bool ok{shader_.CreateShader(editor_.GetText())};
  editorInfo_.CompileTime =
      static_cast<float>((glfwGetTime() - start) * 1000.0f);

  if (ok) {
    shader_.SetUniform("uTexture",
                       0); // new shader program so have to add again
  }
}

void Albedo::SaveSession_() noexcept {

     namespace fs = std::filesystem;

     if (editor_.GetUndoIndex() == 0)
     {
       return;  // yeah,yeah made huge changes....
     }

  
  const std::chrono::zoned_time now{
         std::chrono::current_zone(),
         std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now())};
  const std::string timestamp{std::format("{:%Y-%m-%d_%H-%M-%S}", now)};

    const fs::path dir{"history"};
     std::error_code ec{};
    fs::create_directories(dir, ec);

   std::ofstream out(dir / (timestamp + ".frag.shader"), std::ios::binary);
    if (out) {
      out << editor_.GetText();
    } else {
      Rem::LogError("Couldn't write to file");
    }

}
