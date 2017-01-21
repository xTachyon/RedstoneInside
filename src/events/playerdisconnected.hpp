#ifndef REDI_EVENTS_PLAYERDISCONNECTED_HPP
#define REDI_EVENTS_PLAYERDISCONNECTED_HPP

#include "event.hpp"

namespace redi
{

struct EventPlayerDisconnected : public Event
{
  Player& player;
  
  EventPlayerDisconnected(Player& player) : Event(EventType::PlayerDisconnected), player(player) {}
  
};
  
} // namespace redi

#endif // REDI_EVENTS_PLAYERDISCONNECTED_HPP