#pragma once

#include "../packet.hpp"

namespace redi {
class Server;
}

namespace redi::packets {

struct Ping : public Packet {
  std::int64_t payload;

  Ping(std::int64_t payload);
  Ping(PacketReader& packet);

  void read(PacketReader& packet) override;
  virtual void process(PacketHandler& handler) override;
};

struct Pong : public Packet {
  static constexpr std::int32_t SendID = 0x01;

  std::int64_t payload;

  Pong(std::int64_t payload);

  void write(ByteBuffer& buffer) override;
};

struct Request : public Packet {
  Request() = default;
  Request(PacketReader& packet);

  void read(PacketReader& packet) override;
  virtual void process(PacketHandler& handler) override;
};

struct Response : public Packet {
  static constexpr std::int32_t SendID = 0x00;

  Server& server;

  Response() = default;
  Response(Server& server);

  void write(ByteBuffer& buffer) override;
};

}