#ifndef REDI_EVENTS_DISCONNECT_HPP
#define REDI_EVENTS_DISCONNECT_HPP

#include "../../packet.hpp"

namespace redi
{
namespace packets
{

struct Disconnect : public Packet
{
  static constexpr std::int32_t SendIDlogin = 0x00;
  static constexpr std::int32_t SendIDplay = 0x1A;
  
  std::string json;
  bool play;
  
  Disconnect(const std::string& json, bool play = true);
  Disconnect(std::string&& json, bool play = true);
  
  void write(ByteBuffer& buffer) override;
  
  const char* getName() const override { return REDI_FUNCTION; }
};
  
} // namespace packets
} // namespace redi

#endif // REDI_EVENTS_DISCONNECT_HPP