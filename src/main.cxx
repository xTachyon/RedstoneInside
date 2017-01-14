#include <iostream>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/asio.hpp>
#include "connectionlistener.hpp"
#include "server.hpp"
#include "util/util.hpp"
#include "protocol/packetwriter.hpp"
#include "logger.hpp"

namespace fs = boost::filesystem;
namespace asio = boost::asio;

int main(int, char**)
{
//  redi::PacketWriter writer(0x02);
//  writer.writeString("ecc8e29d-0936-42cb-8492-14ed40555ffd");
//  writer.writeString("Dulcan");
//
//  std::cout << '\n';
//  for (int i = 0; i < writer.data.size(); ++i)
//    std::cout << (int)writer.data[i] << ' ';
//  std::cout << '\n';
//
//  writer.commit();
//
//  std::cout << '\n';
//  for (int i = 0; i < writer.data.size(); ++i)
//    std::cout << (int)writer.data[i] << ' ';
//  std::cout << '\n';
//
//  redi::PacketReader reader(std::move(writer.data));
//  int i = reader.readVarInt();
//  std::string uuid = reader.readString();
//  std::string nick = reader.readString();
  
  asio::io_service io_service;

  try
  {
    redi::Server server(io_service);
    server.run();
  }
  catch (std::exception& e)
  {
    redi::Logger::error(e.what());
  }

  return 0;
}

//enum class State
//{
//  Ok,
//  Error
//};
//
//struct Test
//{
//  int a;
//
//  Test() : a(0) {}
//
//  void increment(State s = State::Ok)
//  {
//    if (s == State::Ok) ++a;
//  }
//};
//
//int main()
//{
//  void (Test::* f)(State) = &Test::increment;
//  Test* a = nullptr;
//
//  (a->*f)(State::Error);
//
//  return 0;
//}