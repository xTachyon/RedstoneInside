#pragma once

#include "../packet.hpp"

namespace redi {
class Server;
}

namespace redi::packets {

struct Ping : public Packet {
  i64 payload;

  explicit Ping(i64 payload);
  explicit Ping(PacketReader& packet);

  void read(PacketReader& packet) override;
  void process(PacketHandler& handler) override;
};

struct Pong : public Packet {
  static constexpr i32 SendID = 0x01;

  i64 payload;

  explicit Pong(i64 payload);

  void write(ByteBuffer& buffer) override;
};

struct Request : public Packet {
  Request() = default;
  explicit Request(PacketReader& packet);

  void read(PacketReader& packet) override;
  void process(PacketHandler& handler) override;
};

struct Response : public Packet {
  static constexpr i32 SendID = 0x00;

  Server& server;

  explicit Response(Server& server);

  void write(ByteBuffer& buffer) override;
};

}