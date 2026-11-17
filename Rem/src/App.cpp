#include <Rem/App.hpp>

// #include <Rem/Renderer/GLUtils.hpp>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <cassert>
#include <iostream>
#include <ranges>

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
      layer->OnUIRender();
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

  // error callback;
}

void App::Shutdown() {

  for (auto &layer : layerStack_) {
    layer->OnDetach();
  }
  layerStack_.clear();

  window_->Destroy();
  glfwTerminate();
  sApp = nullptr;
}
} // namespace Rem