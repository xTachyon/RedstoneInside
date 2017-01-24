#ifndef REDI_ENUMS_HPP
#define REDI_ENUMS_HPP

#include <cstdint>

namespace redi
{
enum class ChatPosition : std::int8_t
{
  ChatBox,
  SystemMessage,
  AboveHotbar
};

} // namespace redi

#endif // REDI_ENUMS_HPP