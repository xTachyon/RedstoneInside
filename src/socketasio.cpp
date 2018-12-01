#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <mutex>
#include "socket.hpp"
#include "bytebuffer.hpp"

namespace asio = boost::asio;

namespace redi {

class AsioSocket : public Socket, public std::enable_shared_from_this<AsioSocket> {
public:
  AsioSocket(asio::ip::tcp::socket&& socket);
  ~AsioSocket() override = default;

  void read(MutableBuffer buffer) override;
  void write(ConstBuffer buffer) override;
private:
  asio::ip::tcp::socket socket;
  asio::io_context::strand strand;

  ByteBuffer current_vector;
  ByteBuffer current_writing;
  std::mutex writing_mutex;

  MutableBuffer read_buffer;

  void writeNext();
  void handleWrite(const boost::system::error_code& error);
  void handleRead(const boost::system::error_code& error, size_t bytes);
};

AsioSocket::AsioSocket(asio::ip::tcp::socket&& socket)
  : socket(std::move(socket)), strand(socket.get_io_context()) {}

void AsioSocket::write(ConstBuffer buffer) {
  {
    std::lock_guard<std::mutex> l(writing_mutex);
    current_vector.append(buffer.data(), buffer.size());
  }
  writeNext();
}

void AsioSocket::writeNext() {
  socket.get_io_service().post(
      strand.wrap([me = shared_from_this(), this] {
        if (!current_writing.empty()) {
          return;
        }

        {
          std::lock_guard<std::mutex> l(writing_mutex);
          current_writing.swap(current_vector);
        }

        if (current_writing.empty()) {
          return;
        }

        asio::async_write(socket,
                          asio::buffer(current_writing.data(), current_writing.size()),
                          boost::bind(&AsioSocket::handleWrite,
                                      shared_from_this(),
                                      asio::placeholders::error));
      }));
}

void AsioSocket::handleWrite(const boost::system::error_code& error) {
  if (error) {

  } else {
    current_writing.clear();
    writeNext();
  }
}

void AsioSocket::read(MutableBuffer buffer) {
  read_buffer = buffer;

  socket.async_read_some(asio::buffer(buffer.data(), buffer.size()),
                   boost::bind(&AsioSocket::handleRead, shared_from_this(),
                               asio::placeholders::error, asio::placeholders::bytes_transferred));
}

void AsioSocket::handleRead(const boost::system::error_code& error, size_t bytes) {
  if (error) {
    read_handler(0, error.message());
  } else {
    read_handler(bytes, "");
  }
}

class AsioListener : public Socket, public std::enable_shared_from_this<AsioListener> {
public:
  AsioListener(asio::io_context& context, uint16_t port);

  void listen();

  void write(ConstBuffer buffer) override;
  void read(MutableBuffer buffer) override;
private:
  asio::ip::tcp::socket socket_to_be_accepted;
  boost::asio::ip::tcp::acceptor acceptor;

  void handleAccept(const boost::system::error_code& error);
};

AsioListener::AsioListener(asio::io_context& context, uint16_t port)
  : socket_to_be_accepted(context), acceptor(context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) {}

void AsioListener::listen() {
  acceptor.async_accept(socket_to_be_accepted,
                        boost::bind(&AsioListener::handleAccept,
                                    shared_from_this(),
                                    asio::placeholders::error));
}

void AsioListener::handleAccept(const boost::system::error_code& error) {
  if (error) {
    accept_handler(nullptr, error.message());
  } else {
    auto socket = std::make_shared<AsioSocket>(std::move(socket_to_be_accepted));
    accept_handler(std::move(socket), "");

    listen();
  }
}

void AsioListener::read(MutableBuffer) {}

void AsioListener::write(ConstBuffer) {}

class AsioNetworking : public Networking {
public:
  explicit AsioNetworking(asio::io_context& context);

  std::shared_ptr<Socket> getListener(socket_accept_handler handler, uint16_t port) override;
private:
  asio::io_context& context;
};

AsioNetworking::AsioNetworking(asio::io_context& context)
  : context(context) {}

std::shared_ptr<Socket> AsioNetworking::getListener(socket_accept_handler handler, uint16_t port) {
  auto listener = std::make_shared<AsioListener>(context, port);
  listener->set_accept_handler(handler);
  listener->listen();
  return listener;
}

}

std::unique_ptr<redi::Networking> getAsioNetworking(boost::asio::io_context& context) {
  return std::make_unique<redi::AsioNetworking>(context);
}