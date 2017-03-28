#ifndef REDI_PACKETS_PLAYERPOSITION_HPP
#define REDI_PACKETS_PLAYERPOSITION_HPP

#include "../../packet.hpp"
#include "../../../../playerposition.hpp"

namespace redi {
namespace packets {

struct PlayerPosition : public Packet {
  double x;
  double y;
  double z;
  bool onGround;

  PlayerPosition(PacketReader& packet);
  PlayerPosition(double x = 0.0, double y = 0.0, double z = 0.0,
                 bool onGround = true);

  void read(PacketReader& packet) override;
  void process(PacketHandler& handler) override;
};

} // namespace packets
} // namespace redi

#endif // REDI_PACKETS_PLAYERPOSITION_HPP