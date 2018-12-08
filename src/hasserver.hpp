#pragma once

namespace redi {

class Server;

class HasServer {
public:
  HasServer(Server& serv)
        : server(serv) {}

  virtual ~HasServer() = default;

  Server& getServer() { return server; }
  const Server& getServer() const { return server; }

protected:
  Server& server;
};

}