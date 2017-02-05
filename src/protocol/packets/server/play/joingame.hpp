#ifndef REDI_JOINGAME_HPP
#define REDI_JOINGAME_HPP

#include "../../packet.hpp"
#include "../../../../player.hpp"

namespace redi
{

struct JoinGame : public Packet
{
  static constexpr std::int32_t SendID = 0x23;
  
  Player* player;
  
  JoinGame(Player* ptr = nullptr);
  
  void write(ByteBuffer& buffer) override;
  
  const char* getName() const override { return REDI_FUNCTION; }
};

} // namespace redi

#endif // REDI_JOINGAME_HPP