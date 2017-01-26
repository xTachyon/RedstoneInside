#ifndef REDI_PROTOCOL_HPP
#define REDI_PROTOCOL_HPP

#include "../bytebuffer.hpp"
#include "packetreader.hpp"
#include "../world/chunk.hpp"
#include "../enums.hpp"

namespace redi
{

class Session;
class Player;

class Protocol
{
public:
  
  Protocol(Session& session, ProtocolVersion version) : mSession(session), mProtocolVersion(version) {}
  
  virtual ~Protocol() = 0;
  
  virtual void handlePacket(ByteBuffer&) {}
  
  virtual void sendStatusPong(std::int64_t) {}
  virtual void sendJoinGame(const Player&) {}
  virtual void sendSetCompression() {}
  virtual void sendLoginSucces(const std::string&, const std::string&) {}
  virtual void sendSpawnPosition() {}
  virtual void sendPlayerAbilities() {}
  virtual void sendPlayerPositionAndLook() {}
  virtual void sendKeepAlive() {}
  virtual void sendTimeUpdate() {}
  virtual void sendChunk(const Chunk&, Vector2i) {}
  
  virtual ByteBuffer createChatPacket(const std::string&, ChatPosition) { return {}; }
  
  ProtocolVersion getProtocolVersion() const { return mProtocolVersion; }
  
protected:

  Session& mSession;
  const ProtocolVersion mProtocolVersion;
};

inline Protocol::~Protocol() {}
  
} // namespace redi

#endif // REDI_PROTOCOL_HPP
