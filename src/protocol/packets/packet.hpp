#ifndef REDI_PACKET_HPP
#define REDI_PACKET_HPP

#include <memory>
#include "../../bytebuffer.hpp"
#include "../packetreader.hpp"
#include "../../util/util.hpp"

namespace redi
{

class PacketHandler;
class Player;
class Session;
using SessionSharedPtr = std::shared_ptr<Session>;

struct Packet
{
  Packet() = default;
  
  virtual ~Packet() = 0;
  
  virtual void read(PacketReader&) {}
  virtual void write(ByteBuffer&) {}
  virtual void process(PacketHandler&) {}
  
  std::string getName() const
  {
    return util::demangleTypeName(typeid(*this).name());
  }
  
  void send(Session& session);
  void send(SessionSharedPtr& session);
  void send(Player& player);
};

using PacketUniquePtr = std::unique_ptr<Packet>;

} // namespace redi

#endif // REDI_PACKET_HPP