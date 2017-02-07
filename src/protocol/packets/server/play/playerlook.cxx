#include "playerlook.hpp"
#include "../../../packetwriternocopy.hpp"
#include "../../packethandler.hpp"

namespace redi
{
namespace packets
{

PlayerLook::PlayerLook(PacketReader& packet)
{
  read(packet);
}

PlayerLook::PlayerLook(float yaw, float pitch, bool onGround)
      : yaw(yaw), pitch(pitch), onGround(onGround) {}

void PlayerLook::read(PacketReader& packet)
{
  yaw = packet.readFloat();
  pitch = packet.readFloat();
  onGround = packet.readBool();
}

void PlayerLook::process(PacketHandler& handler)
{
  handler.handlePlayerLook(*this);
}
  
} // namespace packets
} // namespace redi