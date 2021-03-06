#pragma once

#include "eventtype.hpp"

namespace redi {

struct Event {
  const EventType type;

  Event(EventType type) : type(type) {}

  virtual ~Event() = 0;

  EventType getType() const { return type; }

  template <typename T>
  T& get() {
    T* ptr = dynamic_cast<T*>(this);
    if (ptr)
      return *ptr;
    throw std::runtime_error("Event dynamic cast failed");
  }
};

inline Event::~Event() {}

using EventUniquePtr = std::unique_ptr<Event>;

} // namespace redi