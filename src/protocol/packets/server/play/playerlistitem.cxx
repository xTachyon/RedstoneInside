#include <boost/format.hpp>
#include "playerlistitem.hpp"
#include "../../../packetwriter.hpp"

namespace redi
{
namespace packets
{

PlayerListItem::PlayerListItem(Player& player, PlayerListItemAction action) : player(player), action(action) {}

void PlayerListItem::write(ByteBuffer& buffer)
{
  PacketWriter packet(buffer, SendID);
  
  packet.writeVarInt(action);
  packet.writeVarInt(1);
  packet.writeUUID(player.getUUID());
  
  switch (action)
  {
  case PlayerListItemAction::AddPlayer:
  {
    packet.writeString(player.getUsername()); // name
    
    packet.writeVarInt(0); // number of properties
    
    packet.writeVarInt(player.getGamemode()); // gamemode
    packet.writeVarInt(0); // ping
    packet.writeBool(false); // has display name
  }
    break;
  
  case PlayerListItemAction::UpdateGamemode:
  {
    packet.writeVarInt(player.getGamemode()); // gamemode
  }
    break;
  
  case PlayerListItemAction::UpdateLantecy:
  {
    packet.writeVarInt(0); // ping
  }
    break;
  
  case PlayerListItemAction::UpdateDisplayName:
  {
    packet.writeBool(false); // has display name
  }
    break;
  
  case PlayerListItemAction::RemovePlayer:
    break;
  }
}
  
} // namespace packets
} // namespace redi