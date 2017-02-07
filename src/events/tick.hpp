#ifndef REDI_EVENTS_TICK_HPP
#define REDI_EVENTS_TICK_HPP

#include "event.hpp"

namespace redi
{
namespace events
{

struct EventTick : public Event
{
  EventTick() = default;
};

} // namespace events
} // namespace redi

#endif // REDI_EVENTS_TICK_HPP