#include "statuspackets.hpp"
#include "../../packetwriter.hpp"
#include "../packethandler.hpp"
#include "../../../server.hpp"

namespace redi::packets {

Ping::Ping(i64 payload) : payload(payload) {}

Ping::Ping(PacketReader& packet) { read(packet); }

void Ping::read(PacketReader& packet) { payload = packet.readLong(); }

void Ping::process(PacketHandler& handler) { handler.handleStatusPing(*this); }

Pong::Pong(i64 payload) : payload(payload) {}

void Pong::write(ByteBuffer& buffer) {
  PacketWriter packet(buffer, SendID);

  packet.writeLong(payload);
}

Request::Request(PacketReader& packet) { read(packet); }

void Request::read(PacketReader&) {}

void Request::process(PacketHandler& handler) {
  handler.handleStatusRequest(*this);
}

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
  if (!config.iconb64.empty()) {
    j["favicon"] = config.iconb64;
  }

  for (const auto& player : server.getOnlinePlayers()) {
    nlohmann::json c;

    c["id"] = boost::lexical_cast<std::string>(player->getUUID());
    c["name"] = player->getUsername();

    j["players"]["sample"].push_back(c);
  }

  PacketWriter packet(buffer, SendID);

  packet.writeString(j.dump());
}

}