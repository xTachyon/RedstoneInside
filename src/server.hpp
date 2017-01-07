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
    mConnectedClients.emplace_back(std::move(socket), this);
  }
  void killConnectedSession(Session* th)
  {
    std::lock_guard<std::mutex> l(mConnectedClientsMutex);
    mConnectedClients.remove_if([&](const Session& ar)
                                {
                                  return std::addressof(ar) == th;
                                });
  }
  void run();
  
private:
  
  using SessionList = std::list<Session>;
  using PlayerList = std::list<Player>;
  
  SessionList mConnectedClients;
  std::mutex mConnectedClientsMutex;
  ConnectionListener mListener;
  boost::asio::io_service& mIoService;

  std::int32_t mEntityCount;
};
  
} // namespace redi



#endif // REDI_SERVER