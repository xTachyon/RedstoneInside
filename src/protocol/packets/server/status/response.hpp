#pragma once

#include "../../packet.hpp"

namespace redi {

class Server;

struct Response : public Packet {
  static constexpr std::int32_t SendID = 0x00;

  Server& server;

  Response() = default;
  Response(Server& server);

  void write(ByteBuffer& buffer) override;
};

} // namespace redi