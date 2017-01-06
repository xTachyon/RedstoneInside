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
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/insert_linebreaks.hpp>
#include <boost/archive/iterators/remove_whitespace.hpp>

namespace fs = boost::filesystem;
namespace asio = boost::asio;

int main(int, char**)
{
  using namespace redi;
  using namespace std;
  
  ByteBuffer s("Akahige, So std::vector seems ideal to be be used a stack of int or other types as struct. The growth factor seems to be the criteria for access time of vector. Fixed array is useful only in some scenariors. The other option is a list which is slower than std::vector.");
  
  std::string enc = util::base64::encodeToString(s);
  std::cout << enc << '\n';
  std::string dec = util::base64::decodeToString(enc);
  
  std::cout << '\n' << dec;
  
  return 0;
}
