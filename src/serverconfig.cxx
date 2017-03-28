#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include "serverconfig.hpp"
#include "util/util.hpp"

namespace fs = boost::filesystem;
namespace po = boost::program_options;

namespace redi {

ServerConfig::ServerConfig(std::string&& filepath)
    : configpath(std::move(filepath)) {
  readConfig();
}

void ServerConfig::readConfig() {
  bool notexists = !fs::exists(configpath) || fs::file_size(configpath) == 0;
  if (notexists) {
    std::ofstream(configpath) << "";
    // For whatever reasons, simply std::ofstream(configpath) doesn't work to
    // create new empty file
  }

  int gm, diff;

  po::options_description desc("options");
  desc.add_options()("online",
                     po::value<bool>(&onlineMode)->default_value(false),
                     "If server is in online mode")(
      "max-players", po::value<int>(&maxPlayers)->default_value(20),
      "Maximum number of players")(
      "motd",
      po::value<std::string>(&motd)->default_value("Redi - Highly flammable"),
      "Server description")("gamemode", po::value<int>(&gm)->default_value(1),
                            "Gamemode")(
      "level-type",
      po::value<std::string>(&levelType)->default_value("default"),
      "Level type")("difficulty", po::value<int>(&diff)->default_value(0),
                    "Difficulty")(
      "server-icon",
      po::value<std::string>(&iconpath)->default_value("icon.png"),
      "Server icon path")("port", po::value<int>(&port)->default_value(25565),
                          "Port")(
      "rangeview", po::value<std::uint16_t>(&rangeView)->default_value(5),
      "Range view");

  po::variables_map vm;
  std::ifstream file(configpath);

  po::store(po::parse_config_file(file, desc), vm);
  po::notify(vm);

  gamemode = static_cast<Gamemode>(gm);
  difficulty = static_cast<Difficulty>(diff);
  reducedDebugInfo = false;
  readIcon();

  if (notexists) {
    writeConfig();
  }
}

void ServerConfig::readIcon() {
  if (fs::exists(iconpath)) {
    ByteBuffer buffer;
    buffer.resize(static_cast<std::size_t>(fs::file_size(iconpath)));

    {
      std::ifstream file(iconpath);
      file.read(buffer.as_char(), buffer.size());
    }

    iconb64 = std::string("data:image/png;base64,") +
              util::Base64Encoder::encodeToString(buffer);
  }
}

void ServerConfig::writeConfig() {
  using boost::property_tree::ptree;

  ptree root;

  root.put("online", onlineMode);
  root.put("max-players", maxPlayers);
  root.put("motd", motd);
  root.put("gamemode", static_cast<int>(gamemode));
  root.put("difficulty", static_cast<int>(difficulty));
  root.put("level-type", levelType);
  root.put("port", port);
  root.put("server-icon", "icon.png");
  root.put("rangeview", rangeView);

  write_ini(configpath, root);
}

const char* GamemodeEnumToString(Gamemode g) {
  const char* names[] = {"survival", "creative", "adventure", "spectator"};
  return names[static_cast<std::size_t>(g)];
}

const char* DifficultyEnumToString(Difficulty d) {
  const char* names[] = {"peaceful", "easy", "normal", "hard"};
  return names[static_cast<std::size_t>(d)];
}

} // namespace redi
