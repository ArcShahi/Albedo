#include <Rem/App.hpp>

#include <cassert>
#include <iostream>
#include <ranges>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace Rem {

static App *sApp{nullptr};

App::App(const AppSpecs &specs) : specs_{specs} {

  sApp = this;
  Init();
  // Error Callback
}

App::~App() { Shutdown(); }

void App::Run() noexcept {

  running_ = true;

  float lastTime{GetTime()};

  while (running_) {

    glfwPollEvents();

    if (window_->ShouldClose()) {
      Stop();
      break;
    }

    float currentTime{GetTime()};
    float dt{glm::clamp(currentTime - lastTime, 0.001f, 0.1f)};
    lastTime = currentTime;

    // MAIN LAYER : UPDATE
    for (const auto &layer : layerStack_) {
      layer->OnUpdate(dt);
    }

    // Can use another thread to render
    for (const auto &layer : layerStack_) {
      layer->OnRender();
    }

    //////////////////             IMGUI                           /////////////////////////////////////

     // UI -> default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    const glm::vec2 fb{GetFramebufferSize()};
    glViewport(0, 0, static_cast<int>(fb.x), static_cast<int>(fb.y));
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

    for (const auto &layer : layerStack_)
      layer->OnUIRender();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Only if ViewportsEnable is on
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
      GLFWwindow *backup = glfwGetCurrentContext();
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      glfwMakeContextCurrent(backup);
    }


    window_->Update();

    // Transition here if needed after the updates are done
  }
}

void App::Stop() noexcept { running_ = false; }

void App::RaiseEvent(Event &event) {



  for (auto &layer : std::ranges::views::reverse(layerStack_)) {
    layer->OnEvent(event);
    if (event.handled)
      break;
  }
}

glm::vec2 App::GetFramebufferSize() const {
  return window_->GetFrameBufferSize();
}

App &App::Get() {
  assert(sApp);
  return *sApp;
}

float App::GetTime() { return static_cast<float>(glfwGetTime()); }

void App::Init() {

  glfwInit();

  specs_.windowSpecs.eventCallback = [this](Event &event) {
    RaiseEvent(event);
  };

  window_ = std::make_shared<Window>(specs_.windowSpecs);
  window_->Create();

  //////////////////////////////////////////////////////////////////////////////////////////////////////
  //                             IMGUI                                                              ///
  ////////////////////////////////////////////////////////////////////////////////////////////////////

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io{ImGui::GetIO()};
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  ImGui_ImplGlfw_InitForOpenGL(window_->GetHandle(), true); // your getter name
  ImGui_ImplOpenGL3_Init("#version 460");

  // TODO error callback;
}

void App::Shutdown() {

  for (auto &layer : layerStack_) {
    layer->OnDetach();
  }
  layerStack_.clear();

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  window_->Destroy();
  glfwTerminate();
  sApp = nullptr;
}
} // namespace Rem