#ifndef REDI_SPAWNPOSITION_HPP
#define REDI_SPAWNPOSITION_HPP

#include "../../packet.hpp"
#include "../../../../vectorn.hpp"

namespace redi
{

struct SpawnPosition : public Packet
{
  static constexpr std::int32_t SendID = 0x43;
  
  Vector3i position;
  
  SpawnPosition() = default;
  SpawnPosition(Vector3i position);
  
  void write(ByteBuffer& buffer) override;
  
  const char* getName() const override { return REDI_FUNCTION; }
};
  
} // namespace redi

#endif // REDI_SPAWNPOSITION_HPP