#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/asio.hpp>
#include "protocol/packet.hpp"
#include "logger.hpp"
#include "nbt/nbt.hpp"
#include "region.hpp"
#include "chunk.hpp"
#include "events/event.hpp"
#include "events/eventmanager.hpp"
#include "events/vanillaobserver.hpp"
#include "util/util.hpp"

namespace fs = boost::filesystem;
namespace asio = boost::asio;

void f()
{
  using namespace redi;
  
  std::string str = "valgrind --leak-check=yes myprog arg1 arg2\n";
  std::string enc = util::base64::encodeToString(str);
  std::string dec = util::base64::decodeToString(enc);
  
  std::cout << dec;
}

int main(int, char**)
{
  using namespace redi;
  
  f();
  
  return 0;
}
