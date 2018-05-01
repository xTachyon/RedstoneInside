#ifndef REDI_PACKET_HPP
#define REDI_PACKET_HPP

#include <list>
#include <memory>
#include "../../bytebuffer.hpp"
#include "../packetreader.hpp"
#include "../../util/util.hpp"

namespace redi {

class PacketHandler;
class Player;
class Session;
using SessionSharedPtr = std::shared_ptr<Session>;
using PlayerSharedPtr = std::shared_ptr<Player>;

struct Packet {
  Packet() = default;

  virtual ~Packet() = 0;

  virtual void read(PacketReader&) {}
  virtual void write(ByteBuffer&) {}
  virtual void process(PacketHandler&) {}

  std::string getName() const {
    return util::demangleTypeName(*this);
  }

  void send(Session& session);
  void send(SessionSharedPtr& session);
  void send(Player& player);
  
  void send(std::list<PlayerSharedPtr>& list);
};

using PacketUniquePtr = std::unique_ptr<Packet>;

} // namespace redi

#endif // REDI_PACKET_HPP