#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include "serverconfig.hpp"
#include "util/util.hpp"

namespace fs = boost::filesystem;
namespace po = boost::program_options;

namespace redi
{

ServerConfig::ServerConfig(std::string&& filepath) : configpath(std::move(filepath))
{
  readConfig();
}

void ServerConfig::readConfig()
{
  bool notexists = !fs::exists(configpath) || fs::file_size(configpath) == 0;
  if (notexists)
  {
    std::ofstream(configpath) << "";
    // For whatever reasons, simply std::ofstream(configpath) doesn't work to create new empty file
  }

  int gm, diff;

  po::options_description desc("options");
  desc.add_options()
        ("online", po::value<bool>(&onlineMode)->default_value(false), "If server is in online mode")
        ("max-players", po::value<int>(&maxPlayers)->default_value(20), "Maximum number of players")
        ("motd", po::value<std::string>(&motd)->default_value("Redi - Highly flammable"), "Server description")
        ("gamemode", po::value<int>(&gm)->default_value(1), "Gamemode")
        ("level-type", po::value<std::string>(&levelType)->default_value("default"), "Level type")
        ("difficulty", po::value<int>(&diff)->default_value(0), "Difficulty")
        ("server-icon", po::value<std::string>(&iconpath)->default_value("icon.png"), "Server icon path")
        ("port", po::value<int>(&port)->default_value(25565), "Port");

  po::variables_map vm;
  std::ifstream file(configpath);

  po::store(po::parse_config_file(file, desc), vm);
  po::notify(vm);

  gamemode = static_cast<Gamemode>(gm);
  difficulty = static_cast<Difficulty>(diff);
  readIcon();

  if (notexists)
  {
    writeConfig();
  }
}

void ServerConfig::readIcon()
{
  if (fs::exists(iconpath))
  {
    ByteBuffer buffer;
    buffer.resize(fs::file_size(iconpath));

    {
      std::ifstream file(iconpath);
      file.read(buffer.as_char(), buffer.size());
    }

    iconb64 = std::string("data:image/png;base64,") + util::Base64Encoder::encodeToString(buffer);
  }
  else
  {
    static constexpr char DefaultIcon[] = "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";

    iconb64 = DefaultIcon;
    writeIcon();
    iconb64 = std::string("data:image/png;base64,") + DefaultIcon;
  }
}

void ServerConfig::writeConfig()
{
  using boost::property_tree::ptree;

  ptree root;

  root.put("online", onlineMode);
  root.put("max-players", maxPlayers);
  root.put("motd", motd);
  root.put("gamemode", static_cast<int>(gamemode));
  root.put("difficulty", static_cast<int>(difficulty));
  root.put("level-type", levelType);
  root.put("port", port);

  write_ini(configpath, root);
}

void ServerConfig::writeIcon()
{
  ByteBuffer buffer = util::Base64Decoder::decode(iconb64);
  std::ofstream(iconpath).write(buffer.as_const_char(), buffer.size());
}

const char* GamemodeEnumToString(Gamemode g)
{
  const char* names[] = {"survival", "creative", "adventure", "spectator"};
  return names[static_cast<std::size_t>(g)];
}

const char* DifficultyEnumToString(Difficulty d)
{
  const char* names[] = {"peaceful", "easy", "normal", "hard"};
  return names[static_cast<std::size_t>(d)];
}

} // namespace redi
