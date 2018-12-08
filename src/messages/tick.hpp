#pragma once

#include "event.hpp"

namespace redi {
namespace events {

struct EventTick : public Event {
  EventTick() = default;
};

} // namespace messages
} // namespace redi