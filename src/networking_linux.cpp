#include <array>
#include <utility>
#include <deque>
#include <vector>
#include <memory>
#include <thread>
#include <mutex>
#include <boost/format.hpp>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "networking.hpp"
#include "logger.hpp"

namespace redi {

Socket::Socket() {
  Logger::debug(boost::format("LinuxSocket %1% created") % this);
}

Socket::~Socket() {
  Logger::debug(boost::format("LinuxSocket %1% destroyed") % this);
}

void Socket::set_accept_handler(socket_accept_handler handler) {
  accept_handler = std::move(handler);
}

void Socket::set_read_handler(socket_read_handler handler) {
  read_handler = std::move(handler);
}

void Socket::write(ConstBuffer* buffers, size_t size) {
  for (size_t i = 0; i < size; ++i) {
    write(buffers[i]);
  }
}

#define CHECK(x) {\
    if ((x) < 0) {\
      throw std::runtime_error(\
        (boost::format(__FILE__ ":%1% %2% %3%" ) % __LINE__ % #x % strerror(errno)).str()\
      );\
    }\
  }

#define CHECK_DC(socket, x)\
  if ((x) < 0) {\
    socket->read_handler(0, (boost::format(__FILE__ ":%1% %2%" ) % __LINE__ % strerror(errno)).str());\
    return;\
  }

struct AutoCloseableDescriptor {
  int fd;

  AutoCloseableDescriptor(int fd = -1);
  AutoCloseableDescriptor(AutoCloseableDescriptor&& other) noexcept;
  ~AutoCloseableDescriptor();

  AutoCloseableDescriptor& operator=(AutoCloseableDescriptor&&);

  void close();
};

AutoCloseableDescriptor::AutoCloseableDescriptor(int fd)
  : fd(fd) {
  if (fd != -1) {
    Logger::debug(boost::format("[AutoCloseableDescriptor] Created %1%") % fd);
  }
}

AutoCloseableDescriptor::AutoCloseableDescriptor(redi::AutoCloseableDescriptor&& other) noexcept  {
  fd = other.fd;
  other.fd = -1;
}

AutoCloseableDescriptor::~AutoCloseableDescriptor() {
  if (fd != -1) {
    Logger::debug(boost::format("[AutoCloseableDescriptor] Closed %1%") % fd);
  }
  close();
}

void AutoCloseableDescriptor::close() {
  if (fd != -1) {
    ::close(fd);
  }
  fd = -1;
}

AutoCloseableDescriptor& AutoCloseableDescriptor::operator=(AutoCloseableDescriptor&& other) {
  close();
  fd = other.fd;
  other.fd = -1;
  return *this;
}

class LinuxNetworking;

class LinuxSocket : public Socket {
public:
  LinuxSocket(LinuxNetworking* networking);
  explicit LinuxSocket(int fd, LinuxNetworking* networking);

  int get_fd() const;

  void read(MutableBuffer buffer) override;
  void write(ConstBuffer buffer) override;
  void write(ConstBuffer* buffers, size_t size) override;
  void close() override;

  bool can_read() const;
  void on_read();
private:
  friend class LinuxNetworking;

  LinuxNetworking* networking;
  AutoCloseableDescriptor fd;
  MutableBuffer read_buffer;
  std::deque<byte> write_buffer;
  bool destroyed;
  bool is_listener;
};

Networking::~Networking() = default;

void set_non_blocking(int fd) {
  auto current = fcntl(fd, F_GETFL, 0);
  CHECK(current);
  CHECK(fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK));
}

class LinuxNetworking : public Networking {
public:
  LinuxNetworking();
  ~LinuxNetworking() override;

  std::shared_ptr<Socket> getListener(socket_accept_handler handler, uint16_t port) override;
  void stop() override;
private:
  friend class LinuxSocket;
  friend struct ThreadLockable;

  std::thread thread;
  std::vector<std::shared_ptr<LinuxSocket>> sockets;
  std::recursive_mutex mutex;
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
  LinuxSocket& get_socket(size_t i);
};

LinuxNetworking::LinuxNetworking()
  : running(true) {
  int p[2];
  CHECK(pipe(p));

  reading_pipe = p[0];
  writing_pipe = p[1];
  set_non_blocking(p[0]);
  set_non_blocking(p[1]);

  FD_ZERO(&reading_set);
  FD_ZERO(&writing_set);
  FD_ZERO(&error_set);

  thread = std::thread(&LinuxNetworking::network_thread, this);
}

LinuxNetworking::~LinuxNetworking() {
  thread.join();
}

std::shared_ptr<Socket> LinuxNetworking::getListener(socket_accept_handler handler, uint16_t port) {
  auto listener = std::make_shared<LinuxSocket>(this);
  listener->set_accept_handler(handler);
  listener->is_listener = true;
  set_non_blocking(listener->fd.fd);

  sockaddr_in server{};
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  server.sin_port = htons(port);

  CHECK(bind(listener->fd.fd, (__CONST_SOCKADDR_ARG) &server, sizeof(server)));
  CHECK(listen(listener->fd.fd, 20));

  std::lock_guard<std::recursive_mutex> lock_guard(mutex);
  sockets.push_back(listener);
  CHECK(::write(writing_pipe.fd, "0", 1));

  return listener;
}

void LinuxNetworking::network_thread() {
  try {
    run_loop();
  } catch (std::exception& e) {
    Logger::error(e.what());
  }
}

void LinuxNetworking::run_loop() {
  update_list();
  while (running) {
    {
      std::lock_guard<std::recursive_mutex> lock_guard(mutex);
      handle_events();
      update_list();
    }
    CHECK(select(FD_SETSIZE, &reading_set, &writing_set, &error_set, nullptr));
  }
}

void LinuxNetworking::update_list() {
  FD_ZERO(&reading_set);
  FD_ZERO(&writing_set);
  FD_ZERO(&error_set);
  FD_SET(reading_pipe.fd, &reading_set);

  for (size_t i = 0; i < sockets.size(); ++i) {
    LinuxSocket& socket = *sockets[i];

    if (socket.destroyed) {
      sockets.erase(sockets.begin() + i);
      --i;
      continue;
    }

    if (socket.can_read()) {
      FD_SET(socket.fd.fd, &reading_set);
    }
    if (!socket.write_buffer.empty()) {
      FD_SET(socket.fd.fd, &writing_set);
    }
  }
}

void LinuxNetworking::handle_events() {
  FD_SET(reading_pipe.fd, &reading_set);
  for (size_t i = 0; i < FD_SETSIZE; ++i) {
    if (FD_ISSET(i, &error_set)) {
      LinuxSocket& socket = get_socket(i);
      socket.read_handler(0, "Error");
      continue;
    }

    if (FD_ISSET(i, &reading_set)) {
      if (static_cast<int>(i) == reading_pipe.fd) {
        byte buffer[64 * 1024];
        auto ret = read(reading_pipe.fd, buffer, sizeof(buffer));
        if (ret == -1 && errno != EWOULDBLOCK) {
          CHECK(read(reading_pipe.fd, buffer, sizeof(buffer)));
        }
        continue;
      }

      LinuxSocket& socket = get_socket(i);
      socket.on_read();
    }

    if (FD_ISSET(i, &writing_set)) {
      LinuxSocket& socket = get_socket(i);
      byte buffer[64 * 1024];
      auto to_copy = std::min(sizeof(buffer), socket.write_buffer.size());
      std::copy(socket.write_buffer.begin(), socket.write_buffer.begin() + to_copy, buffer);
      auto wrote = write(socket.fd.fd, buffer, to_copy);
      CHECK(wrote);
//      Logger::info(boost::format("Send %1% bytes") % wrote);
      socket.write_buffer.erase(socket.write_buffer.begin(), socket.write_buffer.begin() + wrote);
    }
  }
}

LinuxSocket& LinuxNetworking::get_socket(size_t i) {
  std::shared_ptr<LinuxSocket>& x = *std::find_if(sockets.begin(), sockets.end(), [i] (auto& socket) {
    return socket->fd.fd == static_cast<int>(i);
  });
  return *x;
}

void LinuxNetworking::stop() {
  std::lock_guard<std::recursive_mutex> lock_guard(mutex);
  running = false;
  CHECK(::write(writing_pipe.fd, "0", 1));
}

LinuxSocket::LinuxSocket(LinuxNetworking* networking)
    : networking(networking), destroyed(false), is_listener(false) {
  fd = socket(AF_INET, SOCK_STREAM, 0);
  CHECK(fd.fd);
}

LinuxSocket::LinuxSocket(int fd, LinuxNetworking* networking)
    : networking(networking), fd(fd), destroyed(false), is_listener(false) {}

int LinuxSocket::get_fd() const {
  return fd.fd;
}

void LinuxSocket::read(MutableBuffer buffer) {
  std::lock_guard<std::recursive_mutex> lock_guard(networking->mutex);
  read_buffer = buffer;
  CHECK(::write(networking->writing_pipe.fd, "0", 1));
}

void LinuxSocket::write(ConstBuffer buffer) {
  std::lock_guard<std::recursive_mutex> lock_guard(networking->mutex);
  write_buffer.insert(write_buffer.end(), buffer.data(), buffer.data() + buffer.size());
  CHECK(::write(networking->writing_pipe.fd, "0", 1));
}

void LinuxSocket::write(ConstBuffer* buffers, size_t size) {
  std::lock_guard<std::recursive_mutex> lock_guard(networking->mutex);
  for (size_t i = 0; i < size; ++i) {
    write(buffers[i]);
  }
}

bool LinuxSocket::can_read() const {
  return read_buffer.size() != 0 || accept_handler;
}

void LinuxSocket::on_read() {
  if (is_listener) {
    int client = accept(fd.fd, nullptr, nullptr);
    CHECK(client);
    set_non_blocking(client);
    auto socket = std::make_shared<LinuxSocket>(client, networking);
    accept_handler(socket, "");

    std::lock_guard<std::recursive_mutex> lock_guard(networking->mutex);
    networking->sockets.push_back(socket);
    CHECK(::write(networking->writing_pipe.fd, "0", 1));
  } else {
    auto read_size = ::read(fd.fd, read_buffer.data(), read_buffer.size());
    if (read_size == 0) {
      destroyed = true;
      return;
    }
    CHECK_DC(this, read_size);
    read_buffer = MutableBuffer();
    read_handler(static_cast<size_t>(read_size), "");
  }
}

void LinuxSocket::close() {
  std::lock_guard<std::recursive_mutex> lock_guard(networking->mutex);
  destroyed = true;
  CHECK(::write(networking->writing_pipe.fd, "0", 1));
}

std::unique_ptr<redi::Networking> getLinuxNetworking() {
  return std::make_unique<LinuxNetworking>();
}

}