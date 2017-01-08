#ifndef REDI_SERVER
#define REDI_SERVER

#include <list>
#include "player.hpp"
#include "connectionlistener.hpp"

namespace redi
{

class Server
{
public:

  Server(boost::asio::io_service& io_service) : mListener(io_service, 25565, this), mIoService(io_service), mEntityCount(0) {}

  std::int32_t getNewEntityID() { return mEntityCount++; }
  void addConnectedSession(boost::asio::ip::tcp::socket&& socket)
  {
    std::lock_guard<std::mutex> l(mConnectedClientsMutex);
    mConnectedClients.emplace_back(std::make_shared<Session>(std::move(socket), this));
    mConnectedClients.back()->setProtocol(mConnectedClients.back());
  }
  void killConnectedSession(Session* th)
  {
    std::lock_guard<std::mutex> l(mConnectedClientsMutex);
    mConnectedClients.remove_if([&](const SessionPtr& ar)
                                {
                                  return std::addressof(*ar) == th;
                                });
  }
  void run();
  void addPacket(Protocol* ptr, ByteBuffer&& buffer);
  void addPlayer(const std::string nick, const std::string uuid, SessionPtr session);
  
private:
  
  using SessionList = std::list<SessionPtr>;
  using PlayerList = std::list<Player>;
  
  SessionList mConnectedClients;
  std::mutex mConnectedClientsMutex;
  ConnectionListener mListener;
  boost::asio::io_service& mIoService;
  ThreadSafeQueue<std::pair<Protocol*, ByteBuffer>> mPacketsToBeHandled;
  std::list<Player> mPlayers;

  std::int32_t mEntityCount;
};
  
} // namespace redi



#endif // REDI_SERVER