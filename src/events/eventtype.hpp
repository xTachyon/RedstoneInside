#ifndef REDI_EVENT_EVENTTYPE_HPP
#define REDI_EVENT_EVENTTYPE_HPP

namespace redi
{

enum class EventType
{
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

#endif // REDI_EVENT_EVENTTYPE_HPP