#pragma once

#include "Event.hpp"

#include <format>

namespace Rem {

class WindowClosedEvent : public Event {
public:
  WindowClosedEvent() {}

  EVENT_CLASS_TYPE(WindowClose)
};

class WindowResizeEvent : public Event {
public:
  WindowResizeEvent(uint32_t width, uint32_t height)
      : width_(width), height_(height) {}

 [[nodiscard]] inline uint32_t GetWidth() const { return width_; }
 [[nodiscard]] inline uint32_t GetHeight() const { return height_; }

 [[nodiscard]] std::string ToString() const override {
    return std::format("WindowResizeEvent: {}, {}", width_, height_);
  }

  EVENT_CLASS_TYPE(WindowResize)
private:
  uint32_t width_, height_;
};
} // namespace Core