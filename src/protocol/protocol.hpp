#ifndef REDI_PROTOCOL_HPP
#define REDI_PROTOCOL_HPP

#include "../bytebuffer.hpp"
#include "packetreader.hpp"

namespace redi
{

class Session;

class Protocol
{
public:
  
  Protocol(Session* player) : mSession(player) {}
  
  virtual void handlePacket(ByteBuffer&) {}
  virtual void handleHandshake(PacketReader&) {}
  virtual void handleStatusRequest(PacketReader&) {}
  virtual void handleStatusPing(PacketReader&) {}
  
  virtual void sendStatusPong(std::int64_t) {}
  
  virtual ~Protocol() = 0;
  
protected:

  Session* mSession;
};

inline Protocol::~Protocol() {}
  
} // namespace redi

#endif // REDI_PROTOCOL_HPP
