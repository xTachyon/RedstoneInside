#include "util/util.hpp"
#include "logger.hpp"
#include "commands/redicommands.hpp"
#include "player.hpp"
#include "server.hpp"

namespace fs = boost::filesystem;

namespace redi {

std::unique_ptr<redi::Networking> getAsioNetworking(boost::asio::io_context& context);
std::unique_ptr<redi::Networking> getLinuxNetworking();

Server::Server()
    : 
//      networking(getAsioNetworking(workIoService)),
      networking(getLinuxNetworking()),
      connectionListener(networking->getListener([this] (std::shared_ptr<Socket> socket, std::string message) {
        this->onSocketConnected(socket, message);
      }, static_cast<uint16_t>(configuration.port))),
      mEntityCount(0),
      mChatManager(*this), mEventManager(*this),
      commandmanager(*this), commands(std::make_unique<commands::RediCommands>(*this)), running(true),
      mUniqueLock(mCondVarMutex) {
  fs::create_directories("players");
  fs::create_directories("worlds");

  addWorld("world", "worlds/world");

//  mListener->listen();

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

void Server::onSocketConnected(std::shared_ptr<redi::Socket> socket, std::string error) {
  auto session = std::make_shared<Session>(std::move(socket), *this);
  addTask([session, this] () { sessions.push_back(session); });
}

void Server::run() {
  while (true) {
    handleOne();
    
    if (!running) {
      break;
    }
    
    mCondVar.wait(mUniqueLock);
  }

  closeServer();
  handleOne();
}

void Server::handleOne() {
  while (true) {
    std::function<void()> x;
    if (!mPacketsToBeHandle.pop(x) || !x)
      break;

    try {
      x();
    } catch (std::exception& e) {
      Logger::error(e.what());
      // Just ignore everything bad.
//      x->getSession().disconnect();
      // and disconnect
      // TODO: add message
    }
  }

  try {
    mEventManager();
  } catch (std::exception&) {
    throw;
  }
}

void Server::addTask(std::function<void()> function) {
  mPacketsToBeHandle.push(std::move(function));
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

void Server::closeServer() {
//  mListener->mIsStopping = true;

  for (auto& i : mPlayers) {
    i->kick("Server is closing");
  }
}

void Server::stop() {
  running = false;
  mCondVar.notify_one();
}

} // namespace redi
