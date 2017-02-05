#ifndef REDI_SERVER
#define REDI_SERVER

#include <list>
#include "player.hpp"
#include "connectionlistener.hpp"
#include "serverconfig.hpp"
#include "events/events.hpp"
#include "world.hpp"
#include "chat/chatmanager.hpp"
#include "events/eventmanager.hpp"
#include "chat/commandmanager.hpp"
#include "chat/redicommands.hpp"
#include "logger.hpp"

namespace redi
{

class Server
{
public:
  
  using PlayerList = std::list<Player>;
  
  ServerConfig config;

  Server();
  ~Server();

  std::int32_t getNewEntityID() { return mEntityCount++; }
  void addConnectedSession(boost::asio::ip::tcp::socket&& socket)
  {
    std::lock_guard<std::mutex> l(mConnectedClientsMutex);
    mStatusConnections.emplace_back(std::make_shared<Session>(std::move(socket), *this));
    mStatusConnections.back()->readNext();
  }
  void run();
  void addPacket(PacketHandlerSharedPtr ptr);
  
  void addEvent(EventSharedPtr ptr);
  void addWorld(const std::string& worldname, const std::string& worlddir);
  std::int32_t getOnlinePlayersNumber() const { return mOnlinePlayers; }
  PlayerList& getOnlinePlayers() { return mPlayers; }
  const PlayerList& getOnlinePlayers() const { return mPlayers; }
  void broadcastPacketToPlayers(ByteBufferSharedPtr ptr, std::function<bool(const Player&)> comp);
  bool getAcceptConnections() const { return mAcceptConnections; }
  void sendMessage(const std::string& str) const { Logger::info(str); }
  Player* findPlayer(const std::string& name);
  EventManager& getEventManager() { return mEventManager; }
  
  static bool toAllPlayers(const Player&) { return true; }
  static bool toAllPlayersExcept(const Player& player, const Player& except);
  
private:
  
  friend class EventManager;
  friend class PacketHandler;
  
  using SessionList = std::list<std::shared_ptr<Session>>;
  using WorldList = std::list<World>;
  
  boost::asio::io_service mIoService;
  SessionList mStatusConnections;
  std::mutex mConnectedClientsMutex;
  ConnectionListener mListener;
  PlayerList mPlayers;
  WorldList mWorlds;
  std::int32_t mEntityCount;
  std::int32_t mOnlinePlayers;
  CommandManager mCommandManager;
  ChatManager mChatManager;
  EventManager mEventManager;
  std::atomic_bool mAcceptConnections;
  RediCommands mRediCommands;
  ThreadSafeQueue<PacketHandlerSharedPtr> mPacketHandlersToBe;
};
  
} // namespace redi

#endif // REDI_SERVER