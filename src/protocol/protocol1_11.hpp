#ifndef REDI_PROTOCOL111_HPP
#define REDI_PROTOCOL111_HPP

#include <boost/asio.hpp>
#include "protocol.hpp"
#include "../bytebuffer.hpp"

namespace redi
{

class Player;

class Protocol1_11 : public Protocol
{
public:
  
  Protocol1_11(SessionPtr ptr) : Protocol(ptr) {}
  
  virtual ~Protocol1_11();
  
  virtual void handlePacket(ByteBuffer& buffer) override;
  virtual void handleHandshake(PacketReader& reader) override;
  virtual void handleStatusRequest(PacketReader& reader) override;
  virtual void handleStatusPing(PacketReader&) override;
  virtual void handleLoginStart(PacketReader&);
  
  virtual void sendStatusPong(std::int64_t number) override;
  virtual void sendLoginSucces(const std::string& nick, const std::string& uuid) override;
  virtual void sendJoinGame(const Player& player) override;
  virtual void sendSetCompression() override;
  
  std::string getIP();
  static std::string getIP(boost::asio::ip::tcp::socket& socket);
};

inline Protocol1_11::~Protocol1_11() {}
  
} // namespace redi

#endif //REDI_PROTOCOL111_HPP
