#ifndef REDI_HASSERVER_HPP
#define REDI_HASSERVER_HPP

namespace redi {

class Server;

class HasServer {
public:
  HasServer(Server& serv)
      : server(serv) {}
  
  Server& getServer() { return server; }
  const Server& getServer() const { return server; }

protected:
  Server& server;
};

}

#endif // REDI_HASSERVER_HPP
