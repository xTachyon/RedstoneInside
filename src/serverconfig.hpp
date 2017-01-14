#ifndef REDI_SERVERCONFIG_HPP
#define REDI_SERVERCONFIG_HPP

#include <string>

namespace redi
{

enum class Gamemode : std::uint8_t
{
  Survival,
  Creative,
  Adventure,
  Spectator
};

enum class Dimension : std::int8_t
{
  Nether = -1,
  Overworld,
  End
};

enum class Difficulty : std::uint8_t
{
  Peaceful,
  Easy,
  Normal,
  Hard
};

enum PlayerAbilitiesFlag : std::int8_t
{
  Invulnerable = 0x01,
  Flying = 0x02,
  AllowFlying = 0x04,
  CreativeMode = 0x08
};

struct ServerConfig
{
  bool onlineMode = false;
  int maxPlayers = 20;
  std::string motd = "Redi - May contain nuts";
  Gamemode gamemode = Gamemode::Creative;
  Difficulty difficulty = Difficulty::Peaceful;
  std::string evelType = "default";
  bool reducedDebugInfo = false;
};
  
} // namespace redi

#endif //REDI_SERVERCONFIG_HPP
