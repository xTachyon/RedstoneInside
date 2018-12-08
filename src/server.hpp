#pragma once

#include <list>
#include <condition_variable>
#include "player.hpp"
#include "serverconfig.hpp"
#include "messages/events.hpp"
#include "world.hpp"
#include "chat/chatmanager.hpp"
#include "messages/eventmanager.hpp"
#include "logger.hpp"
#include "util/threadgroup.hpp"
#include "commands/commandmanager.hpp"
#include "commands/command.hpp"
#include "networking.hpp"

namespace redi {

class Server {
public:
  using PlayerList = std::list<PlayerSharedPtr>;

  static constexpr std::size_t AsioThreadsNumber = 1;
  /*
   * Minimum 1
  */

  Server();
  ~Server();

  std::int32_t getNewEntityID() { return mEntityCount++; }
  
  void run();
  void addTask(std::function<void()> function);

  void addEvent(EventUniquePtr&& ptr);
  void addWorld(const std::string& worldname, const std::string& worlddir);
  
  std::int32_t getOnlinePlayersNumber() const { return std::distance(mPlayers.begin(), mPlayers.end()); }
  PlayerList& getOnlinePlayers() { return mPlayers; }
  const PlayerList& getOnlinePlayers() const { return mPlayers; }
  void sendMessage(const std::string& str) const { Logger::info(str); }
  Player* findPlayer(const std::string& name);
  EventManager& getEventManager() { return mEventManager; }
  ChatManager& getChatManager() { return mChatManager; }
  boost::asio::io_service& getWorkIO() { return workIoService; }
  
  commands::CommandManager& getCommandManager() { return commandmanager; }
  
  ServerConfig& getServerConfiguration() { return configuration; }
  
  const ServerConfig& getServerConfiguration() const { return configuration; }

  void stop();
private:
  friend class EventManager;
  friend class PacketHandler;

  using SessionList = std::list<std::shared_ptr<Session>>;
  using WorldList = std::list<World>;
  
  ServerConfig configuration;
  
  boost::asio::io_context workIoService;
  boost::asio::io_context::work workWorkIoService;
  std::unique_ptr<Networking> networking;
  std::shared_ptr<Socket> connectionListener;
  PlayerList mPlayers;
  WorldList mWorlds;
  std::int32_t mEntityCount;
  ChatManager mChatManager;
  EventManager mEventManager;
  lockfree::Queue<std::function<void()>> mPacketsToBeHandle;
  commands::CommandManager commandmanager;
  std::unique_ptr<commands::Command> commands;
  bool running;
  SessionList sessions;
  
  util::ThreadGroup<std::thread> asiothreads;
  std::condition_variable mCondVar;
  std::mutex mCondVarMutex;
  std::unique_lock<std::mutex> mUniqueLock;

  void handleOne();
  void closeServer();

  void onSocketConnected(std::shared_ptr<Socket> socket, std::string error);
};

} // namespace redi