#pragma once

#include "Event.hpp"

#include <concepts>
#include <memory>
#include <utility>

namespace Rem {

class Layer {
public:
  virtual ~Layer() = default;

  virtual void OnEvent([[maybe_unused]] Event &event) {}
  virtual void OnUpdate([[maybe_unused]] float ts) {}
  virtual void OnRender() {}

  template <std::derived_from<Layer> T, typename... Args>
  void TransitionTo(Args &&...args) {
    QueueTransition(std::make_unique<T>(std::forward<Args>(args)...));
  }

private:
  void QueueTransition_(std::unique_ptr<Layer> layer);
};

} // namespace Rem