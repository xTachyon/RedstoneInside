#include <json.hpp>
#include "../../../../server.hpp"
#include "response.hpp"
#include "../../../packetwriter.hpp"

namespace redi {

Response::Response(Server& server) : server(server) {}

void Response::write(ByteBuffer& buffer) {
  const ServerConfig& config = server.getServerConfiguration();
  nlohmann::json j;

  j["description"]["text"] = config.motd;
  j["version"]["name"] = "RedstoneInside";
  j["version"]["protocol"] = 316;
  j["players"]["max"] = config.maxPlayers;
  j["players"]["online"] = server.getOnlinePlayersNumber();
  j["players"]["sample"] = nlohmann::json::array();
  if (config.iconb64.size() != 0)
    j["favicon"] = config.iconb64;

  for (const auto& player : server.getOnlinePlayers()) {
    nlohmann::json c;

    c["id"] = boost::lexical_cast<std::string>(player->getUUID());
    c["name"] = player->getUsername();

    j["players"]["sample"].push_back(c);
  }

  PacketWriter packet(buffer, SendID);

  packet.writeString(j.dump());
}

} // namespace redi