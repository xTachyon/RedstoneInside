#ifndef REDI_PLAYERLOOK_HPP
#define REDI_PLAYERLOOK_HPP

#include "event.hpp"

namespace redi
{

struct EventPlayerPosition : public Event
{
  float yaw;
  float pitch;
  bool onGround;
  
  EventPlayerPosition(float yaw, float pitch, bool onGround) : Event(EventType::PlayerLook), yaw(yaw), pitch(pitch), onGround(onGround) {}
};
  
} // namespace redi

#endif //REDI_PLAYERLOOK_HPP
