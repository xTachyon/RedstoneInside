#ifndef REDI_PROTOCOL_HPP
#define REDI_PROTOCOL_HPP

#include "../bytebuffer.hpp"
#include "packetreader.hpp"

namespace redi
{

class Session;
class Player;

class Protocol
{
public:
  
  Protocol(Session* session) : mSession(session) {}
  
  virtual void handlePacket(ByteBuffer&) {}
  
  virtual void sendStatusPong(std::int64_t) {}
  virtual void sendJoinGame(const Player&) {}
  virtual void sendSetCompression() {}
  virtual void sendLoginSucces(const std::string&, const std::string&) {}
  virtual void sendSpawnPosition() {}
  virtual void sendPlayerAbilities() {}
  virtual void sendPlayerPositionAndLook() {}
  virtual void sendKeepAkive() {}
  
  virtual ~Protocol() = 0;
  
protected:

  Session* mSession;
};

inline Protocol::~Protocol() {}
  
} // namespace redi

#endif // REDI_PROTOCOL_HPP
