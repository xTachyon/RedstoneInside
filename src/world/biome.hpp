#ifndef REDI_WORLD_BIOME_HPP
#define REDI_WORLD_BIOME_HPP

#include <cstdint>

namespace redi
{

enum class Biome : std::uint8_t
{
  Ocean = 0,
  Plains = 1,
  Desert = 2,
  ExtremeHills = 3,
  Forest = 4
};
  
} // namespace redi

#endif // REDI_WORLD_BIOME_HPP