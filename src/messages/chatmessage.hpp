#ifndef REDI_EVENT_CHATMESSAGE_HPP
#define REDI_EVENT_CHATMESSAGE_HPP

#include "event.hpp"
#include "../player.hpp"

namespace redi {

struct EventChatMessage : public Event {
  Player& player;
  std::string message;

  EventChatMessage(Player& player, std::string&& message)
      : Event(EventType::ChatMessage), player(player),
        message(std::move(message)) {}
};

} // namespace redi

#endif // REDI_EVENT_CHATMESSAGE_HPP