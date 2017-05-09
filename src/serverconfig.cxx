#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include "serverconfig.hpp"
#include "util/util.hpp"
#include "filesystem.hpp"
#include "logger.hpp"

namespace po = boost::program_options;

namespace redi {

const char* ServerConfig::ConfigFilePath = "configuration.txt";
const char* ServerConfig::DefaultConfigFileContent =
    "# Hello !\n"
        "online=false\n"
        "maxplayers=1472\n"
        "motd=Redi - Highly flammable\n"
        "gamemode=1\n"
        "difficulty=0\n"
        "leveltype=DEFAULT\n"
        "port=25565\n"
        "icon=icon.png\n"
        "rangeview=5";

ServerConfig::ServerConfig() {
  onlineMode = false;
  maxPlayers = 1472;
  motd = "Redi - Highly flammable";
  gamemode = Gamemode::Creative;
  difficulty = Difficulty::Peaceful;
  levelType = "DEFAULT";
  reducedDebugInfo = false;
  port = 25565;
  iconpath = "icon.png";
  rangeView = 5;
  
  if (fs::exists(ConfigFilePath)) {
    try {
      readConfig();
    }
    catch (std::exception&) {
      Logger::error("Error while parsing the configuration file");
      throw;
    }
  }
  else {
    writeConfig();
  }
}

void ServerConfig::readConfig() {
  static const char* OnlineText = "online";
  static const char* MaxPlayerText = "maxplayers";
  static const char* MotdText = "motd";
  static const char* GamemodeText = "gamemode";
  static const char* DifficultyText = "difficulty";
  static const char* LevelTypeText = "leveltype";
  static const char* PortText = "port";
  static const char* IconText = "icon";
  static const char* RangeViewText = "rangeview";
  
  boost::property_tree::ptree tree;
  
  /*
   * Just a tree, keep scrolling.
              _{\ _{\{\/}/}/}__
             {/{/\}{/{/\}(\}{/\} _
            {/{/\}{/{/\}(_)\}{/{/\}  _
         {\{/(\}\}{/{/\}\}{/){/\}\} /\}
        {/{/(_)/}{\{/)\}{\(_){/}/}/}/}
       _{\{/{/{\{/{/(_)/}/}/}{\(/}/}/}
      {/{/{\{\{\(/}{\{\/}/}{\}(_){\/}\}
      _{\{/{\{/(_)\}/}{/{/{/\}\})\}{/\}
     {/{/{\{\(/}{/{\{\{\/})/}{\(_)/}/}\}
      {\{\/}(_){\{\{\/}/}(_){\/}{\/}/})/}
       {/{\{\/}{/{\{\{\/}/}{\{\/}/}\}(_)
      {/{\{\/}{/){\{\{\/}/}{\{\(/}/}\}/}
       {/{\{\/}(_){\{\{\(/}/}{\(_)/}/}\}
         {/({/{\{/{\{\/}(_){\/}/}\}/}(\}
          (_){/{\/}{\{\/}/}{\{\)/}/}(_)
            {/{/{\{\/}{/{\{\{\(_)/}
             {/{\{\{\/}/}{\{\\}/}
              {){/ {\/}{\/} \}\}
              (_)  \.-'.-/
          __...--- |'-.-'| --...__
   _...--"   .-'   |'-.-'|  ' -.  ""--..__
 -"    ' .  . '    |.'-._| '  . .  '   jro
 .  '-  '    .--'  | '-.'|    .  '  . '
          ' ..     |'-_.-|
  .  '  .       _.-|-._ -|-._  .  '  .
              .'   |'- .-|   '.
  ..-'   ' .  '.   `-._.-´   .'  '  - .
   .-' '        '-._______.-'     '  .
        .      ~,
    .       .   |\   .    ' '-.
   */
  
  boost::property_tree::ini_parser::read_ini(ConfigFilePath, tree);
  
  if (tree.count(OnlineText)) {
    onlineMode = tree.get<bool>(OnlineText);
  }
  if (tree.count(MaxPlayerText)) {
    maxPlayers = tree.get<int>(MaxPlayerText);
  }
  if (tree.count(MotdText)) {
    motd = tree.get<std::string>(MotdText);
  }
  if (tree.count(GamemodeText)) {
    gamemode = static_cast<Gamemode>(tree.get<int>(GamemodeText));
  }
  if (tree.count(DifficultyText)) {
    difficulty = static_cast<Difficulty>(tree.get<int>(DifficultyText));
  }
  if (tree.count(LevelTypeText)) {
    levelType = tree.get<std::string>(LevelTypeText);
  }
  if (tree.count(PortText)) {
    port = tree.get<int>(PortText);
  }
  if (tree.count(IconText)) {
    iconpath = tree.get<std::string>(IconText);
    readIcon();
  }
  if (tree.count(RangeViewText)) {
    rangeView = tree.get<std::uint16_t>(RangeViewText);
  }
}

/*
 * Reads the server icon from disk.
 */
void ServerConfig::readIcon() {
  if (fs::exists(iconpath)) {
    ByteBuffer buffer = util::readFile(iconpath);
    
    iconb64 = std::string("data:image/png;base64,") +
              util::Base64Encoder::encodeToString(buffer);
    /*
     * Icon has to be sent encoded as base64.
     */
  }
}

/*
 * Writes the default configuration file.
 */
void ServerConfig::writeConfig() {
  std::ofstream(ConfigFilePath) << DefaultConfigFileContent;
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
