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
  
  operator std::string&() { return get(); }
  
  operator std::string() const { return get(); }
  
  std::string& get() { return string; }
  
  const std::string& get() const { return string; }
  
  std::string generate() const;
private:
  std::string string;
};

ChatComponent& operator+=(ChatComponent& l, const std::string& r);

ChatComponent operator+(ChatComponent l, const std::string& r);

}
} // namespace redi

#endif // REDI_CHAT_CHATCOMPONENT_HPP