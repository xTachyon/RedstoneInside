#include <utility>

#include <thread>
#include "networking.hpp"

namespace redi {

Socket::~Socket() = default;

void Socket::set_accept_handler(socket_accept_handler handler) {
  accept_handler = std::move(handler);
}

void Socket::write(ByteBuffer buffer) {
  write(ConstBuffer(buffer));
}

void Socket::set_read_handler(socket_read_handler handler) {
  read_handler = std::move(handler);
}

class LinuxSocket : public Socket {
public:
  explicit LinuxSocket(int fd);

  int get_fd() const;
private:
  int fd;
};

LinuxSocket::LinuxSocket(int fd)
  : fd(fd) {}

int LinuxSocket::get_fd() const {
  return fd;
}

Networking::~Networking() = default;

class LinuxNetworking : public Networking {
public:
  LinuxNetworking();

private:
  std::thread thread;

  void network_thread();
};

LinuxNetworking::LinuxNetworking() {
  thread = std::thread(&LinuxNetworking::network_thread, this);
}

void LinuxNetworking::network_thread() {

}



}