#ifndef REDI_EVENTS_PLAYERJOINED_HPP
#define REDI_EVENTS_PLAYERJOINED_HPP

#include <string>
#include "event.hpp"
#include "../player.hpp"

namespace redi
{

struct EventPlayerJoin : public Event
{
  Session& session;
  std::string nick;
  
  EventPlayerJoin(Session& session, std::string&& nick) : Event(EventType::PlayerJoin),session(session), nick(std::move(nick)) {}
};

} // namespace redi

#endif // REDI_EVENTS_PLAYERJOINED_HPP