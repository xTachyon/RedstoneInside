#include <array>
#include <utility>
#include <vector>
#include <memory>
#include <thread>
#include <mutex>
#include <unistd.h>
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

#define CHECK(x) { if ((x) < 0) { throw std::runtime_error(#x); } }

struct AutoCloseableDescriptor {
  int fd;

  explicit AutoCloseableDescriptor(int fd = -1);
  AutoCloseableDescriptor(AutoCloseableDescriptor&& other) noexcept;
  ~AutoCloseableDescriptor();

  AutoCloseableDescriptor& operator=(int fd);

  void close();
};

AutoCloseableDescriptor::AutoCloseableDescriptor(int fd)
  : fd(fd) {}

AutoCloseableDescriptor::AutoCloseableDescriptor(redi::AutoCloseableDescriptor&& other) noexcept  {
  fd = other.fd;
  other.fd = -1;
}

AutoCloseableDescriptor::~AutoCloseableDescriptor() {
  close();
}

AutoCloseableDescriptor& AutoCloseableDescriptor::operator=(int fd) {
  close();
  this->fd = fd;
  return *this;
}

void AutoCloseableDescriptor::close() {
  if (fd != -1) {
    ::close(fd);
  }
  fd = -1;
}

class LinuxSocket : public Socket {
public:
  LinuxSocket();
  explicit LinuxSocket(int fd);

  int get_fd() const;

  void read(MutableBuffer buffer) override;
  void write(ConstBuffer buffer) override;

private:
  friend class LinuxNetworking;

  AutoCloseableDescriptor fd;
  bool destroyed;
};

LinuxSocket::LinuxSocket()
  : destroyed(false) {
}

LinuxSocket::LinuxSocket(int fd)
  : fd(fd) {}

int LinuxSocket::get_fd() const {
  return fd.fd;
}

void LinuxSocket::read(MutableBuffer buffer) {

}

void LinuxSocket::write(ConstBuffer buffer) {

}

Networking::~Networking() = default;

enum class Action {
  Add,
  Modify,
  Remove,
};

struct SocketInfo {
  std::shared_ptr<LinuxSocket> socket;
  uint32_t oldevent;
  Action action;

  explicit SocketInfo(std::shared_ptr<LinuxSocket> socket);
};

SocketInfo::SocketInfo(std::shared_ptr<redi::LinuxSocket> socket)
  : socket(std::move(socket)), oldevent(0), action(Action::Add) {}

class LinuxNetworking : public Networking {
public:
  LinuxNetworking();
  ~LinuxNetworking() override;

  std::shared_ptr<Socket> getListener(socket_accept_handler handler, uint16_t port) override;
private:
  std::thread thread;
  std::vector<SocketInfo> sockets;
  std::mutex mutex;
  bool running;
  AutoCloseableDescriptor reading_pipe;
  AutoCloseableDescriptor writing_pipe;
  fd_set reading_set;
  fd_set writing_set;
  fd_set error_set;

  void network_thread();
  void run_loop();
  void update_list();
  void handle_events();
};

LinuxNetworking::LinuxNetworking()
  : running(true) {
  thread = std::thread(&LinuxNetworking::network_thread, this);

  int p[2];
  CHECK(pipe(p));

  reading_pipe = p[0];
  writing_pipe = p[1];

  memset(&reading_set, 0, sizeof(reading_set));
  memset(&writing_set, 0, sizeof(writing_set));
  FD_ZERO(&error_set);
}

LinuxNetworking::~LinuxNetworking() {
  thread.join();
}

std::shared_ptr<Socket> LinuxNetworking::getListener(socket_accept_handler handler, uint16_t port) {
  auto listener = std::make_shared<LinuxSocket>();
  listener->set_accept_handler(handler);
  return listener;
}

void LinuxNetworking::network_thread() {
  try {

  } catch (...) {}
}

void LinuxNetworking::run_loop() {
  while (running) {
    update_list();
    CHECK(select(FD_SETSIZE, &reading_set, &writing_set, &error_set, nullptr));

  }
}

void LinuxNetworking::update_list() {
  FD_SET(reading_pipe.fd, &reading_set);
  for (size_t i = 0; i < sockets.size(); ++i) {
    SocketInfo& socket_info = sockets[i];
    LinuxSocket& socket = *socket_info.socket;


  }
}

void LinuxNetworking::handle_events() {
  FD_SET(reading_pipe.fd, &reading_set);
  for (size_t i = 0; i < FD_SETSIZE; ++i) {

  }
}

std::unique_ptr<redi::Networking> getLinuxNetworking() {
  return std::make_unique<LinuxNetworking>();
}

}