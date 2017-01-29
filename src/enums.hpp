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

enum class ConnectionState
{
  Handshake,
  Status,
  Login,
  Play
};

const char* getStateName(ConnectionState s);

enum class ProtocolVersion
{
  V1_9_3_4 = 110,
  V1_10_X = 210,
  V1_11 = 315,
  V1_11_2 = 316
};

enum class ChatAction
{
  PlayerLeft,
  PlayerJoined
};

enum class PlayerListItemAction
{
  AddPlayer,
  UpdateGamemode,
  UpdateLantecy,
  UpdateDisplayName,
  RemovePlayer
};

} // namespace redi

#endif // REDI_ENUMS_HPP