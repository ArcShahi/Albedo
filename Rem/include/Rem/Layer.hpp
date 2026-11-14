#pragma once

#include "Event.hpp"

#include <concepts>
#include <memory>
#include <utility>

namespace Rem {

class Layer {
public:
  virtual ~Layer() = default;

  virtual void OnEvent(Event &event) {}
  virtual void OnUpdate(float ts) {}
  virtual void OnRender() {}

  template <std::derived_from<Layer> T, typename... Args>
  void TransitionTo(Args &&...args) {
    QueueTransition(std::make_unique<T>(std::forward<Args>(args)...));
  }

private:
  void QueueTransition_(std::unique_ptr<Layer> layer);
};

} // namespace Rem