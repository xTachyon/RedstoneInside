#pragma once

#include <functional>
#include <memory>
#include "buffers.hpp"

namespace redi {

class Socket;

using socket_accept_handler = std::function<void(std::shared_ptr<Socket>, std::string)>;
using socket_read_handler = std::function<void(size_t, std::string)>;

class Socket {
public:
  Socket();
  virtual ~Socket() = 0;

  virtual void read(MutableBuffer buffer) = 0;
  virtual void write(ConstBuffer buffer) = 0;

  void set_accept_handler(socket_accept_handler handler);
  void set_read_handler(socket_read_handler handler);
protected:
  socket_accept_handler accept_handler;
  socket_read_handler read_handler;
};

using SocketSharedPtr = std::shared_ptr<Socket>;

class Networking {
public:
  virtual ~Networking() = 0;

  virtual std::shared_ptr<Socket> getListener(socket_accept_handler handler, uint16_t port) = 0;
};

}