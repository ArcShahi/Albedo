#pragma once

#include <functional>
#include <iostream>
#include <set>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>
#include <concepts>

#include <Rem/Event.hpp>
#include <Rem/Layer.hpp>
#include <Rem/Window.hpp>

#include <glm/glm.hpp>

namespace Rem {

struct AppSpecs {
  std::string name{"MyApp"};
  WindowSpecs windowSpecs{};
};

class App {
public:
  App(const AppSpecs &specs);
  ~App();

  void Run() noexcept;
  void Stop() noexcept;
  void RaiseEvent(Event& event);

  template <typename TLayer>
    requires(std::is_base_of_v<Layer, TLayer>)
    void PushLayer() {
    layerStack_.emplace_back(std::make_shared<TLayer>())->OnAttach();
  }
  void PushLayer(const std::shared_ptr<Layer> &layer) {
    layerStack_.emplace_back(layer);
    layer->OnAttach();
  }

  [[nodiscard]] glm::vec2 GetFramebufferSize() const;

  [[nodiscard]] std::shared_ptr<Window> GetWindowHandle() const { return window_; }

  static App &Get();
  [[nodiscard]] static float GetTime();

private:
  void Init();
  void Shutdown();

private:
  AppSpecs specs_{};
  std::vector<std::shared_ptr<Layer>> layerStack_{};
  std::shared_ptr<Window> window_{};
  bool running_{false};

  friend class Layer;
};

} // namespace Rem
