#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <nlohmann/json.hpp>
#include "eventmanager.hpp"
#include "../server.hpp"
#include "../util/util.hpp"
#include "../player.hpp"
#include "../protocol/packets/server/play/playerlistitem.hpp"
#include "../protocol/packets/server/login/setcompression.hpp"
#include "../protocol/packets/server/login/loginsucces.hpp"
#include "../protocol/packets/server/play/joingame.hpp"
#include "../protocol/packets/server/play/spawnposition.hpp"
#include "../protocol/packets/server/play/timeupdate.hpp"

namespace redi {

EventManager::EventManager(Server& server) : mServer(server) {}

void EventManager::operator()() {
  while (!mEvents.empty()) {
    EventUniquePtr e = mEvents.pop();
    if (!e)
      continue;

    switch (e->getType()) {
    case EventType::PlayerJoin: {
      EventPlayerJoin& event = e->get<EventPlayerJoin>();
      handlePlayerJoin(event);
      mServer.mChatManager(event);
    } break;

    case EventType::PlayerDisconnect: {
      EventPlayerDisconnect& event = e->get<EventPlayerDisconnect>();
      mServer.mChatManager(event);

      {
        for (PlayerSharedPtr& player : mServer.mPlayers) {
          if (*player != event.player) {
            packets::PlayerListItem(event.player,
                                    PlayerListItemAction::RemovePlayer)
                .send(*player);
          }
        }
      }

      handlePlayerDisconnect(event);
    } break;

    case EventType::SessionDisconnect:
      handleSessionDisconnect(e->get<EventSessionDisconnect>());
      break;

    case EventType::ChatMessage:
      handleChatMessage(e->get<EventChatMessage>());
      break;

    case EventType::ChunkLoaded: {
      handleChunkLoaded(e->get<events::EventChunkLoaded>());
    } break;

    default:
      break;
    }
  }
}

void EventManager::addEvent(EventUniquePtr&& ptr) {
  mEvents.push(std::move(ptr));
}

void EventManager::handlePlayerJoin(EventPlayerJoin& packet) {
  packet.session->setConnectionState(ConnectionState::Play);

  boost::uuids::uuid namesp = boost::lexical_cast<boost::uuids::uuid>(
      "77e7c416-763c-4967-8291-6698b795e90a");
  boost::uuids::name_generator gen(namesp);
  boost::uuids::uuid uuid = gen(util::toLowercase(packet.username));

  Session& mSession = *packet.session;

//  for (SessionSharedPtr& s :
//       mServer.mStatusConnections) // TODO: find a better way
//  {
//    if (s == packet.session) {
//      mServer.mPlayers.emplace_back(std::make_shared<Player>(
//          packet.username, uuid, std::move(s), mServer.getNewEntityID(),
//          mServer, &mServer.mWorlds.back()));
//      mServer.mStatusConnections.remove_if(
//          [](const SessionSharedPtr& par) -> bool {
//            return !static_cast<bool>(par);
//          });
//      break;
//    }
//  }
  
  mServer.mPlayers.emplace_back(std::make_shared<Player>(
      packet.username, uuid, packet.session, mServer.getNewEntityID(),
      mServer, &mServer.mWorlds.back()));

  auto& player = *mServer.mPlayers.back();

  player.getSession().setPlayer(player);
  player.getWorld().addPlayer(player);

  SetCompression(-1).send(mSession);
  LoginSucces(player.getUUIDasString(), player.getUsername()).send(mSession);
  JoinGame(&player).send(mSession);
  SpawnPosition(Vector3i(0, 50, 0)).send(mSession);
  packets::PlayerPositionAndLook(player.getPosition(),
                                 player.getNewTeleportID())
      .send(mSession);
  packets::TimeUpdate(player.getWorld()).send(mSession);
  player.timersNext();

  for (PlayerSharedPtr& idx : mServer.mPlayers) {
    if (*idx == player) {
      for (PlayerSharedPtr& i : mServer.mPlayers) {
        packets::PlayerListItem(*i, PlayerListItemAction::AddPlayer)
            .send(player.getSession());
      }
    } else {
      packets::PlayerListItem(player, PlayerListItemAction::AddPlayer)
          .send(idx->getSession());
    }
  }

  player.onUpdateChunks();
}

void EventManager::handlePlayerDisconnect(EventPlayerDisconnect& event) {
  Player& player = event.player;

  player.getWorld().deletePlayer(player);
  // First remove the player from the world
  // so we won't SIGSEGV when deferencing it after deleting
  mServer.mPlayers.remove_if(
      [&](const PlayerSharedPtr& p) { return *p == player; });
}

void EventManager::handleSessionDisconnect(EventSessionDisconnect& event) {
  mServer.sessions.remove_if(
      [&](const SessionSharedPtr& ar) { return event.session == *ar; });
}

void EventManager::handleChatMessage(EventChatMessage& event) {
  mServer.mChatManager(event);
}

void EventManager::handleChunkLoaded(events::EventChunkLoaded& event) {
  event.region.addChunkAndNotifyPlayers(event.coordinates,
                                        std::move(event.chunk));
}

} // namespace redi