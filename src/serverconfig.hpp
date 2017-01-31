#ifndef REDI_SERVERCONFIG_HPP
#define REDI_SERVERCONFIG_HPP

#include <string>
#include "bytebuffer.hpp"

namespace redi
{

enum class Gamemode : std::uint8_t
{
  Survival,
  Creative,
  Adventure,
  Spectator
};

const char* GamemodeEnumToString(Gamemode);

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

const char* DifficultyEnumToString(Difficulty);

enum PlayerAbilitiesFlag : std::int8_t
{
  Invulnerable = 0x01,
  Flying = 0x02,
  AllowFlying = 0x04,
  CreativeMode = 0x08
};

struct ServerConfig
{
  bool onlineMode;
  int maxPlayers;
  std::string motd;
  Gamemode gamemode;
  Difficulty difficulty;
  std::string levelType;
  bool reducedDebugInfo;
  std::string iconb64;
  int port;

  std::string configpath;
  std::string iconpath;

  ServerConfig(std::string&& filepath);

  void readConfig();
  void readIcon();
  void writeConfig();
};

} // namespace redi

#endif //REDI_SERVERCONFIG_HPP
