#include <Rem/App.hpp>

// #include <Rem/Renderer/GLUtils.hpp>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <cassert>
#include <iostream>

namespace Rem {

static App *sApp{nullptr};

App::App(const AppSpecs &specs) : specs_{specs} {

  sApp = this;

  // Error Callback

  glfwInit();

  specs_.windowSpecs.eventCallback = [this](Event &event) {
    RaiseEvent(event);
  };

  window_ = std::make_shared<Window>(specs_.windowSpecs);
  window_->Create();

  // error callback;
}

App::~App() {
  window_->Destroy();
  glfwTerminate();
  sApp = nullptr;
}

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
    window_->Update();
  }
}

void App::Stop() noexcept { running_ = false; }

void App::RaiseEvent(Event &event) {

  for (auto itr{layerStack_.end() - 1}; itr != layerStack_.begin(); --itr) {
    (*itr)->OnEvent(event);
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

} // namespace Rem