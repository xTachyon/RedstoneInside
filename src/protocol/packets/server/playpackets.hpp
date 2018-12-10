#pragma once

#include "../packet.hpp"
#include "../../../enums.hpp"
#include "../../../world/chunk.hpp"
#include "../../../world.hpp"
#include "../../packetwriter.hpp"
#include "../../../playerposition.hpp"

namespace redi::packets {

struct ChatMessage : public Packet {
  static constexpr std::int32_t SendID = 0x0F;

  std::string message;
  ChatPosition position;

  ChatMessage(const std::string& message = "",
              ChatPosition position = ChatPosition::ChatBox);
  ChatMessage(std::string&& message, ChatPosition position);
  ChatMessage(PacketReader& packet);

  void read(PacketReader& packet) override;
  void write(ByteBuffer& buffer) override;
  virtual void process(PacketHandler& handler) override;
};

struct ChunkData : public Packet {
  static constexpr std::int32_t SendID = 0x20;

  const Chunk& chunk;
  Vector2i position;

  ChunkData(const Chunk& chunk, Vector2i position);

  void write(ByteBuffer& buffer) override;
};

struct Disconnect : public Packet {
  static constexpr std::int32_t SendIDlogin = 0x00;
  static constexpr std::int32_t SendIDplay = 0x1A;

  std::string json;
  bool play;

  Disconnect(const std::string& json, bool play = true);
  Disconnect(std::string&& json, bool play = true);

  void write(ByteBuffer& buffer) override;
};

struct EntityLookAndRelativeMove : public Packet {
  static constexpr std::int32_t SendID = 0x26;

  std::int32_t EID;
  std::int16_t deltax;
  std::int16_t deltay;
  std::int16_t deltaz;
  float yaw;
  float pitch;
  bool onGround;

  EntityLookAndRelativeMove(std::int32_t EID = 0, std::int16_t deltax = 0,
                            std::int16_t deltay = 0, std::int16_t deltaz = 0,
                            float yaw = 0, float pitch = 0,
                            bool onGround = true);

  void write(ByteBuffer& buffer) override;
};

struct JoinGame : public Packet {
  static constexpr std::int32_t SendID = 0x23;

  Player* player;

  JoinGame(Player* ptr = nullptr);

  void write(ByteBuffer& buffer) override;
};

struct KeepAlive : public Packet {
  static constexpr std::int32_t SendID = 0x1F;

  std::int32_t keepAliveID;

  KeepAlive(std::int32_t keepAliveID);

  void write(ByteBuffer& buffer) override;
};

struct PlayerListItem : public Packet {
  static constexpr std::int32_t SendID = 0x2D;

  Player& player;
  PlayerListItemAction action;

  PlayerListItem(Player& player, PlayerListItemAction action);

  void write(ByteBuffer& buffer) override;
};

struct PlayerLook : public Packet {
  float yaw;
  float pitch;
  bool onGround;

  PlayerLook(PacketReader& packet);
  PlayerLook(float yaw = 0.0f, float pitch = 0.0f, bool onGround = true);

  void read(PacketReader& packet) override;
  void process(PacketHandler& handler) override;
};

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

struct PlayerPositionAndLook : public Packet {
  static constexpr std::int32_t SendID = 0x2E;

  double x;
  double y;
  double z;
  float yaw;
  float pitch;
  bool onGround;
  std::int32_t teleportID;

  PlayerPositionAndLook(PacketReader& packet);
  PlayerPositionAndLook(double x = 0.0, double y = 0.0, double z = 0.0,
                        float yaw = 0.0f, float pitch = 0.0f,
                        bool onGround = true, std::int32_t teleportID = 0);

  PlayerPositionAndLook(redi::PlayerPosition position, std::int32_t teleportID);

  void read(PacketReader& packet) override;
  void write(ByteBuffer& buffer) override;
  void process(PacketHandler& handler) override;
};

struct SpawnPlayer : public Packet {
  static constexpr std::int32_t SendID = 0x05;

  Player& player;

  SpawnPlayer(Player& player);

  void write(ByteBuffer& buffer) override;
};

struct SpawnPosition : public Packet {
  static constexpr std::int32_t SendID = 0x43;

  Vector3i position;

  SpawnPosition() = default;
  SpawnPosition(Vector3i position);

  void write(ByteBuffer& buffer) override;
};

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

struct UnloadChunk : public Packet {
  static constexpr std::int32_t SendID = 0x1D;

  Vector2i position;

  UnloadChunk(Vector2i position);

  void write(ByteBuffer& buffer) override;
};

}