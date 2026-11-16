#pragma once

#include <functional>
#include <iostream>
#include <set>
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
    layerStack_.push_back(std::make_unique<TLayer>());
  }

  template <typename TLayer>
    requires(std::is_base_of_v<Layer, TLayer>)
  [[nodiscard]] TLayer *GetLayer() {
    for (const auto &layer : layerStack_) {
      if (auto casted = dynamic_cast<TLayer *>(layer.get()))
        return casted;
    }
    return nullptr;
  }

  [[nodiscard]] glm::vec2 GetFramebufferSize() const;

  [[nodiscard]] std::shared_ptr<Window> GetWindow() const { return window_; }

  static App &Get();
  [[nodiscard]] static float GetTime();

private:
  AppSpecs specs_{};
  std::vector<std::unique_ptr<Layer>> layerStack_{};
  std::shared_ptr<Window> window_{};
  bool running_{false};

  friend class Layer;
};

} // namespace Rem
