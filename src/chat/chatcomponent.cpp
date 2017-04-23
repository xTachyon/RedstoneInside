#include "chatcomponent.hpp"

namespace redi {
namespace chat {

ChatComponent::ChatComponent(std::string&& str)
    : string(std::move(str)) {}

ChatComponent::ChatComponent(const std::string& str)
    : string(str) {}

std::string ChatComponent::generate() const {
  using namespace std::string_literals;
  return "{\"text\":\""s + get() + "\"}";
}

ChatComponent& operator+=(ChatComponent& l, const std::string& r) {
  l.get().append(r.data(), r.size());
  return l;
}

ChatComponent operator+(ChatComponent l, const std::string& r) {
  l += r;
  return l;
}
}
} // namespace redi