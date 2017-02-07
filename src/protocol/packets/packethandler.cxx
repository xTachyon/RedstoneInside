#include <string>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>
#include "../../session.hpp"
#include "packethandler.hpp"
#include "../../util/util.hpp"
#include "../../server.hpp"
#include "../../player.hpp"
#include "server/login/setcompression.hpp"
#include "server/login/loginsucces.hpp"
#include "server/status/response.hpp"
#include "server/status/pong.hpp"
#include "server/play/joingame.hpp"
#include "server/play/spawnposition.hpp"
#include "server/play/playerpositionandlook.hpp"
#include "server/play/chunkdata.hpp"
#include "server/play/playerlistitem.hpp"

namespace redi
{

PacketHandler::PacketHandler(Server& server, Session& session, EventManager& eventh)
      : mServer(server), mSession(session), mEventManager(eventh) {}

void PacketHandler::addPacket(PacketUniquePtr&& packet)
{
  std::lock_guard<std::mutex> l(mMutex);
  mPackets.emplace_back(std::move(packet));
}

void PacketHandler::readRaw(ByteBuffer buffer)
{
  PacketReader packet(buffer);
  std::int32_t type = packet.readVarInt();
  PacketUniquePtr ptr;
  
  switch (mSession.getConnectionState())
  {
  case ConnectionState::Handshake:
  {
    switch (type)
    {
    case 0x00:
    {
      Handshake handshake(packet);
      handleHandshake(handshake);
      return;
    }
      break;
    
    default:
      break;
    }
  }
  
  case ConnectionState::Login:
  {
    switch (type)
    {
    case 0x00:
    {
      LoginStart ls(packet);
      handleLoginStart(ls);
      return;
    }
      break;
    
    default:
      break;
    }
  }
    break;
  
  case ConnectionState::Status:
  {
    switch (type)
    {
    case 0x00:
      ptr = std::make_unique<Request>(packet);
      break;
      
    case 0x01:
      ptr = std::make_unique<Ping>(packet);
      break;
    
    default:
      break;
    }
  }
    break;
  
  case ConnectionState::Play:
  {
    switch (type)
    {
    case 0x02:
      ptr = std::make_unique<packets::ChatMessage>(packet);
      break;
    
    case 0x0D:
      ptr = std::make_unique<packets::PlayerPositionAndLook>(packet);
      break;
      
    case 0x0C:
      ptr = std::make_unique<packets::PlayerPosition>(packet);
      break;
      
    case 0x0E:
      ptr = std::make_unique<packets::PlayerLook>(packet);
      break;
      
    default:
      break;
    }
  }
    break;
  }
  
  addPacket(std::move(ptr));
}

void PacketHandler::handleOne()
{
  PacketUniquePtr ptr;
  
  {
    std::lock_guard<std::mutex> l(mMutex);
    if (mPackets.empty()) return;
    
    ptr = std::move(mPackets.front());
    mPackets.pop_front();
  }
  
  if (ptr) ptr->process(*this);
}

void PacketHandler::handleHandshake(Handshake& p)
{
  mSession.mConnectionState = p.state;
}

void PacketHandler::handleStatusRequest(Request&)
{
  Response(mServer).send(mSession);
}

void PacketHandler::handleStatusPing(Ping& packet)
{
  Pong(packet.payload).send(mSession);
  mSession.disconnect();
}

void PacketHandler::handleLoginStart(LoginStart& packet)
{
  mSession.mConnectionState = ConnectionState::Play;
  
  boost::uuids::uuid namesp = boost::lexical_cast<boost::uuids::uuid>("77e7c416-763c-4967-8291-6698b795e90a");
  boost::uuids::name_generator gen(namesp);
  boost::uuids::uuid uuid = gen(util::toLowercase(packet.username));
  
  for (SessionSharedPtr& s : mServer.mStatusConnections) // TODO: find a better way
  {
    if (*s == mSession)
    {
      mServer.mPlayers.emplace_back(packet.username, uuid, std::move(s), mServer.getNewEntityID(), &mServer, &mServer.mWorlds.back());
      mServer.mStatusConnections.remove_if([](const SessionSharedPtr& par) -> bool
                                           {
                                             return !static_cast<bool>(par);
                                           });
      break;
    }
  }
  Player& player = mServer.mPlayers.back();
  ++mServer.mOnlinePlayers;
  
  player.getSession().setPlayer(player);
  player.getWorld().addPlayer(player);
  
  SetCompression(-1).send(mSession);
  LoginSucces(player.getUUIDasString(), player.getUsername()).send(mSession);
  JoinGame(&player).send(mSession);
  SpawnPosition(Vector3i(0, 50, 0)).send(mSession);
  packets::PlayerPositionAndLook(player.getPosition(), player.getNewTeleportID()).send(mSession);


  ChunkManager& cm = mServer.mWorlds.back().getChunkManager();
  for (std::int32_t i = -2; i <= 2; ++i)
  {
    for (std::int32_t j = -2; j <= 2; ++j)
    {
      Vector2i r(i, j);

      packets::ChunkData(cm.getChunk(r), r).send(mSession);
    }
  }
  
  mServer.addEvent(std::make_shared<EventPlayerJoin>(mSession, std::move(packet.username)));

  for (Player& idx : mServer.mPlayers)
  {
    if (idx == player)
    {
      for (Player& i : mServer.mPlayers)
      {
//        player.getProtocol().sendPlayerListItem(i, PlayerListItemAction::AddPlayer);
        packets::PlayerListItem(i, PlayerListItemAction::AddPlayer).send(player.getSession());
      }
    }
    else
    {
      packets::PlayerListItem(player, PlayerListItemAction::AddPlayer).send(idx.getSession());
//      idx.getProtocol().sendPlayerListItem(player, PlayerListItemAction::AddPlayer);
//      idx.getProtocol().sendSpawnPlayerPacket(player);
    }
  }
}

void PacketHandler::handleChatMessage(packets::ChatMessage& packet)
{
  mServer.addEvent(std::make_shared<EventChatMessage>(mSession.getPlayer(), std::move(packet.message)));
}

void PacketHandler::handlePlayerPositionAndLook(packets::PlayerPositionAndLook& packet)
{
  PlayerPosition& position = mSession.getPlayer().mPosition;
  
  position.x = packet.x;
  position.y = packet.y;
  position.z = packet.z;
  position.yaw = packet.yaw;
  position.pitch = packet.pitch;
  position.onGround = packet.onGround;
}

void PacketHandler::handlePlayerPosition(packets::PlayerPosition& packet)
{
  PlayerPosition& position = mSession.getPlayer().mPosition;
  
  position.x = packet.x;
  position.y = packet.y;
  position.z = packet.z;
  position.onGround = packet.onGround;
}

void PacketHandler::handlePlayerLook(packets::PlayerLook& packet)
{
  PlayerPosition& position = mSession.getPlayer().mPosition;
  
  position.yaw = packet.yaw;
  position.pitch = packet.pitch;
  position.onGround = packet.onGround;
}
  
} // namespace redi