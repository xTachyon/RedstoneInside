#ifndef REDI_SERVER
#define REDI_SERVER

#include <list>
#include "player.hpp"
#include "connectionlistener.hpp"
#include "serverconfig.hpp"
#include "events/events.hpp"
#include "world.hpp"
#include "chatmanager.hpp"
#include "events/eventmanager.hpp"

namespace redi
{

class Server
{
public:
  
  using PlayerList = std::list<Player>;
  
  ServerConfig config;

  Server(boost::asio::io_service& io_service);

  std::int32_t getNewEntityID() { return mEntityCount++; }
  void addConnectedSession(boost::asio::ip::tcp::socket&& socket)
  {
    std::lock_guard<std::mutex> l(mConnectedClientsMutex);
    mStatusConnections.emplace_back(std::make_unique<Session>(std::move(socket), *this));
  }
  void run();
  void addPacket(Protocol* ptr, ByteBuffer&& buffer);
  
  void addEvent(EventSharedPtr ptr);
  void addWorld(const std::string& worldname, const std::string& worlddir);
  std::int32_t getOnlinePlayersNumber() const { return mOnlinePlayers; }
  PlayerList& getOnlinePlayers() { return mPlayers; }
  const PlayerList& getOnlinePlayers() const { return mPlayers; }
  void broadcastPacketToPlayers(ByteBufferSharedPtr ptr, std::function<bool(const Player&)> comp);
  
  static bool toAllPlayers(const Player&) { return true; }
  static bool toAllPlayersExcept(const Player& player, const Player& except);
  
private:
  
  friend class EventManager;
  
  using SessionList = std::list<std::unique_ptr<Session>>;
  using WorldList = std::list<World>;
  
  SessionList mStatusConnections;
  std::mutex mConnectedClientsMutex;
  ConnectionListener mListener;
  boost::asio::io_service& mIoService;
  ThreadSafeQueue<std::pair<Protocol*, ByteBuffer>> mPacketsToBeHandled;
  PlayerList mPlayers;
  WorldList mWorlds;
  std::int32_t mEntityCount;
  std::int32_t mOnlinePlayers;
  ChatManager mChatManager;
  EventManager mEventManager;
};
  
} // namespace redi

#endif // REDI_SERVER