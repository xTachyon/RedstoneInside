#ifndef REDI_PROTOCOL111_HPP
#define REDI_PROTOCOL111_HPP

#include <boost/asio.hpp>
#include "protocol.hpp"
#include "../bytebuffer.hpp"
#include "../logger.hpp"

namespace redi
{

class Player;

class Protocol1_11 : public Protocol
{
public:
  
  Protocol1_11(Session* ptr) : Protocol(ptr) {}
  
  virtual ~Protocol1_11();
  
  virtual void handlePacket(ByteBuffer& buffer) override;
  virtual void handleHandshake(PacketReader& reader);
  virtual void handleStatusRequest(PacketReader& reader);
  virtual void handleStatusPing(PacketReader&);
  virtual void handleLoginStart(PacketReader&);
  virtual void handleClientSettings(PacketReader&);
  virtual void handlePlayerBlockPlacement(PacketReader&);
  virtual void handleChatMessage(PacketReader&);
  
  virtual void sendStatusPong(std::int64_t number) override;
  virtual void sendLoginSucces(const std::string& nick, const std::string& uuid) override;
  virtual void sendJoinGame(const Player& player) override;
  virtual void sendSetCompression() override;
  virtual void sendSpawnPosition() override;
  virtual void sendPlayerAbilities() override;
  virtual void sendPlayerPositionAndLook() override;
  virtual void sendKeepAlive() override;
  virtual void sendTimeUpdate() override;
  virtual void sendChunk(const Chunk&, Vector2i pos) override;
  
  std::string getIP();
  static std::string getIP(boost::asio::ip::tcp::socket& socket);
};

inline Protocol1_11::~Protocol1_11()
{
  Logger::info((boost::format("Protocol %1% destroyed") % this).str());
}
  
} // namespace redi

#endif //REDI_PROTOCOL111_HPP
