#pragma once

#include <cstdint>
#include <functional>
#include <memory>

namespace redi {

enum class EventPriority : std::uint8_t {
  Lowest,
  Low,
  Normal,
  High,
  Highest,
  Monitor
};

} // namespace redi