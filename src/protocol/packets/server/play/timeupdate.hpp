#ifndef REDI_PACKETS_TIMEUPDATE_HPP
#define REDI_PACKETS_TIMEUPDATE_HPP

#include "../../packet.hpp"
#include "../../../../world.hpp"
#include "../../../packetwriter.hpp"

namespace redi {
namespace packets {

struct TimeUpdate : public Packet {
  static constexpr std::int32_t SendID = 0x44;

  const World& world;

  TimeUpdate(const World& world) : world(world) {}

  void write(ByteBuffer& buffer) override {
    PacketWriter packet(buffer, SendID);

    packet.writeLong(world.getWorldTime());
    packet.writeLong(world.getWorldTime() % 24'000);
  }
};

} // namespace packets
} // namespace redi

#endif // REDI_PACKETS_TIMEUPDATE_HPP