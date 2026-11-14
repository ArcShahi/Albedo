#pragma once

#include <functional>
#include <string>

namespace Rem {

enum class EventType {

  None = 0,
  WindowClose,
  WindowResize,
  KeyPressed,
  KeyReleased,
  MouseButtonPressed,
  MouseButtonReleased,
  MouseMoved,
  MouseScrolled

};

#define EVENT_CLASS_TYPE(type)\
  static EventType GetStaticType() { return EventType::type;}\
  virtual EventType GetEventType() const override { return GetStaticType(); }\
  virtual const char *GetName() const override { return #type; }\

class Event {
public:
  virtual ~Event() {}
  virtual EventType GetEventType() const = 0;
  virtual const char *GetName() const= 0;
  virtual std::string ToString() const { return GetName(); }

 public:
  bool handled{false};
};


class EventDispatcher {
	template<typename T>
	using EventFunc=std::function<bool(T&)>;

public:
        EventDispatcher(Event &event) : event_{event} {}

	   template <typename T> bool Dispatch(EventFunc<T> func) {
          if (event_.GetEventType() == T::GetStaticType() && !event_.handled) {
               event_.handled = func(*(T *)&event_);
            return true;
          }
          return false;
	   }

private:
        Event& event_;
};




} // namespace Rem