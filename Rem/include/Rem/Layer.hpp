#pragma once

#include "Event.hpp"

#include <concepts>
#include <memory>
#include <utility>

namespace Rem {

class Layer {
public:
  virtual ~Layer() = default;

  virtual void OnAttach() {}
  virtual void OnDetach() {}
  virtual void OnEvent([[maybe_unused]] Event &event) {}
  virtual void OnUpdate([[maybe_unused]] float ts) {}
  virtual void OnUIRender() {}
  virtual void OnRender() {}

};

} // namespace Rem