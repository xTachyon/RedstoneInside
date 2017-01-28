#ifndef REDI_PLAYERPOSITIONANDLOOK_HPP
#define REDI_PLAYERPOSITIONANDLOOK_HPP

#include "event.hpp"
#include "../vectorn.hpp"

namespace redi
{

struct EventPlayerPosition : public Event
{
  Vector3d position;
  bool onGround;
  
  EventPlayerPosition(float yaw, float pitch, bool onGround) : Event(EventType::PlayerPositionAndLook), yaw(yaw), pitch(pitch), onGround(onGround) {}
};
  
} // namespace redi

#endif //REDI_PLAYERPOSITIONANDLOOK_HPP
