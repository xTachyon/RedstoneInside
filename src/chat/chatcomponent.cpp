#include "chatcomponent.hpp"

namespace redi {
namespace chat {

ChatComponent::ChatComponent(std::string&& str)
    : string(std::move(str)) {}

ChatComponent::ChatComponent(string_view str)
    : string(str) {}

std::string ChatComponent::generate() const {
  using namespace std::string_literals;
  return "{\"text\":\""s + get() + "\"}";
}

ChatComponent::ChatComponent(const std::string& str)
    : ChatComponent(string_view(str)) {}

ChatComponent& operator+=(ChatComponent& l, string_view r) {
  l.get().append(r.data(), r.size());
  return l;
}

ChatComponent operator+(ChatComponent l, string_view r) {
  l += r;
  return l;
}
}
} // namespace redi