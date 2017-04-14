#include "util/util.hpp"
#include "logger.hpp"
#include "exceptions.hpp"
#include "player.hpp"
#include "server.hpp"

namespace fs = boost::filesystem;

namespace redi {

Server::Server()
    : config("server.properties"),
      mListener(std::make_shared<ConnectionListener>(
          workIoService, static_cast<std::uint16_t>(config.port), *this)),
      mEntityCount(0),
      mChatManager(*this), mEventManager(*this),
      commandmanager(*this), mRediCommands(*this),
      mUniqueLock(mCondVarMutex) {
  fs::create_directories("players");
  fs::create_directories("worlds");

  addWorld("world", "worlds/world");

  mListener->listen();

  asiothreads.create(AsioThreadsNumber, [&] () {
    Logger::debug((boost::format("Asio work thread id %1% started") %
                   std::this_thread::get_id())
                      .str());

    workIoService.run();

    Logger::debug((boost::format("Asio work thread id %1% stopped") %
                   std::this_thread::get_id())
                      .str());
  });

  Logger::info("Redi has started");
}

Server::~Server() {
  Logger::debug("Redi is stopping");

  workIoService.stop();
}

void Server::run() {
  using namespace std::chrono_literals;

  while (true) {
    while (true) {
      PacketHandlerSharedPtr x;
      if (!mPacketsToBeHandle.pop(x) || !x)
        break;

      try {
        x->handleOne();
      } catch (std::exception& e) {
        Logger::error(e.what());
        // Just ignore everything bad.
        x->getSession().disconnect();
        // and disconnect
        // TODO: add message
      }
    }

    try {
      mEventManager();
    } catch (StopServer&) {
      closeServer("Server is closing");
      mEventManager();
      return;
    } catch (std::exception&) {
      throw;
    }

    mCondVar.wait(mUniqueLock);
  }
}

void Server::addPacket(PacketHandlerSharedPtr ptr) {
  mPacketsToBeHandle.push(ptr);
  mCondVar.notify_one();
}

void Server::addEvent(EventUniquePtr&& ptr) {
  mEventManager.addEvent(std::move(ptr));
  mCondVar.notify_one();
}

void Server::addWorld(const std::string& worldname,
                      const std::string& worlddir) {
  mWorlds.emplace_back(*this, worldname, worlddir,
                       std::make_shared<TerrainGenerator>());
}

void Server::broadcastPacketToPlayers(ByteBufferSharedPtr ptr,
                                      std::function<bool(const Player&)> comp) {
  std::for_each(mPlayers.begin(), mPlayers.end(), [&](PlayerSharedPtr& player) {
    if (comp(*player)) {
      player->sendPacket(ByteBuffer(*ptr));
    }
  });
}

Player* Server::findPlayer(const std::string& name) {
  Player* ptr = nullptr;

  for (auto& index : mPlayers) {
    if (util::noCaseCompareEqual(name, index->getUsername())) {
      ptr = index.get();
      break;
    }
  }

  return ptr;
}

void Server::closeServer(const std::string& reason) {
  mListener->mIsStopping = true;

  for (auto& i : mPlayers) {
    i->kick(reason);
  }
}

bool Server::toAllPlayersExcept(const Player& player, const Player& except) {
  return player != except;
}

} // namespace redi
