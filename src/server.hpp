#ifndef REDI_SERVER
#define REDI_SERVER

#include <list>
#include "player.hpp"
#include "connectionlistener.hpp"
#include "serverconfig.hpp"
#include "event.hpp"
#include "world.hpp"

namespace redi
{

class Server
{
public:
  
  ServerConfig config;

  Server(boost::asio::io_service& io_service);

  std::int32_t getNewEntityID() { return mEntityCount++; }
  void addConnectedSession(boost::asio::ip::tcp::socket&& socket)
  {
    std::lock_guard<std::mutex> l(mConnectedClientsMutex);
    mConnectedClients.emplace_back(std::move(socket), this);
  }
  void run();
  void addPacket(Protocol* ptr, ByteBuffer&& buffer);
  void addPlayer(const std::string nick, Session* session);
  void addEvent(EventPtr ptr);
  void addWorld(const std::string& worldname, const std::string& worlddir);
  
private:
  
  using SessionList = std::list<Session>;
  using PlayerList = std::list<Player>;
  using WorldList = std::list<World>;
  
  SessionList mConnectedClients;
  std::mutex mConnectedClientsMutex;
  ConnectionListener mListener;
  boost::asio::io_service& mIoService;
  ThreadSafeQueue<std::pair<Protocol*, ByteBuffer>> mPacketsToBeHandled;
  PlayerList mPlayers;
  ThreadSafeQueue<EventPtr> mActions;
  WorldList mWorlds;

  std::int32_t mEntityCount;
};
  
} // namespace redi

#endif // REDI_SERVER