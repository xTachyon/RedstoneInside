#ifndef REDI_CHAT_CHATCOMPONENT_HPP
#define REDI_CHAT_CHATCOMPONENT_HPP

#include "../datatypes.hpp"

namespace redi {
namespace chat {

// TODO: fix later
class ChatComponent {
public:
  ChatComponent() = default;
  
  ChatComponent(const std::string& str);
  ChatComponent(std::string&& str);
  ChatComponent(string_view str);
  
  operator std::string&() { return get(); }
  
  operator string_view() const { return get(); }
  
  std::string& get() { return string; }
  
  string_view get() const { return string; }
  
  std::string generate() const;
private:
  std::string string;
};

ChatComponent& operator+=(ChatComponent& l, string_view r);

ChatComponent operator+(ChatComponent l, string_view r);

}
} // namespace redi

#endif // REDI_CHAT_CHATCOMPONENT_HPP