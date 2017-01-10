#ifndef REDI_PROTOCOL_HPP
#define REDI_PROTOCOL_HPP

#include "../bytebuffer.hpp"
#include "packetreader.hpp"

namespace redi
{

class Session;
class Player;
using SessionPtr = std::shared_ptr<Session>;

class Protocol
{
public:
  
  Protocol(SessionPtr player) : mSession(player) {}
  
  virtual void handlePacket(ByteBuffer&) {}
  virtual void handleHandshake(PacketReader&) {}
  virtual void handleStatusRequest(PacketReader&) {}
  virtual void handleStatusPing(PacketReader&) {}
  
  virtual void sendStatusPong(std::int64_t) {}
  virtual void sendJoinGame(const Player&) {}
  virtual void sendSetCompression() {}
  virtual void sendLoginSucces(const std::string&, const std::string&) {}
  
  virtual ~Protocol() = 0;
  
protected:

  SessionPtr mSession;
};

inline Protocol::~Protocol() {}
  
} // namespace redi

#endif // REDI_PROTOCOL_HPP
