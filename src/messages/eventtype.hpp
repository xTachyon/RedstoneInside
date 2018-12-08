#pragma once

namespace redi {

enum class EventType {
  PlayerJoin,
  PlayerDisconnect,
  SessionDisconnect,
  SendKeepAliveRing,
  ChatMessage,
  StatusRequest,
  PlayerPosition,
  PlayerLook,
  PlayerPositionAndLook,
  ChunkLoaded,
  ChunkUnloaded,
  RegionLoaded,
  RegionUnloaded
};

} // namespace redi