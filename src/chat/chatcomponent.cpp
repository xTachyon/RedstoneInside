#include "chatcomponent.hpp"

namespace redi {
namespace chat {

ChatComponent::ChatComponent(std::string&& str)
    : string(std::move(str)) {}

ChatComponent::ChatComponent(string_view str)
    : string(str) {}
  
}
} // namespace redi