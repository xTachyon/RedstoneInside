#ifndef REDI_CHAT_CHATCOMPONENT_HPP
#define REDI_CHAT_CHATCOMPONENT_HPP

#include "../datatypes.hpp"

namespace redi {
namespace chat {

// TODO: fix later
class ChatComponent {
public:
  ChatComponent() = default;
  
  ChatComponent(std::string&& str);
  
  ChatComponent(string_view str);

private:
  std::string string;
};

}
} // namespace redi

#endif // REDI_CHAT_CHATCOMPONENT_HPP