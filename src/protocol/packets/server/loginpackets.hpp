#pragma once

#include "../packet.hpp"

namespace redi::packets {

struct LoginStart : public Packet {
  std::string username;

  LoginStart() = default;
  LoginStart(PacketReader& packet);
  LoginStart(const std::string& username);
  LoginStart(std::string&& username);

  void read(PacketReader& packet) override;
  virtual void process(PacketHandler& handler) override;
};

struct LoginSucces : public Packet {
  static constexpr std::int32_t SendID = 0x02;

  std::string uuid;
  std::string username;

  LoginSucces() = default;
  LoginSucces(const std::string& uuid, const std::string& username);
  LoginSucces(std::string&& uuid, std::string&& username);

  void write(ByteBuffer& buffer) override;
};

struct SetCompression : public Packet {
  static constexpr std::int32_t SendID = 0x03;

  std::int32_t threshold;

  SetCompression() = default;
  SetCompression(std::int32_t threshold);

  void write(ByteBuffer& buffer) override;
};

}