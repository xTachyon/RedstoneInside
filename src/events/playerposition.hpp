#ifndef REDI_PLAYERPOSITION_HPP
#define REDI_PLAYERPOSITION_HPP

#include "event.hpp"
#include "../vectorn.hpp"

namespace redi
{

struct EventPlayerPosition : public Event
{
  Vector3d position;
  bool onGround;
  
  EventPlayerPosition(Vector3d position, bool onGround) : Event(EventType::PlayerPosition), position(position), onGround(onGround) {}
};
  
} // namespace redi

#endif //REDI_PLAYERPOSITION_HPP
