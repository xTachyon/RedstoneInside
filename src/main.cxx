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

namespace fs = boost::filesystem;
namespace asio = boost::asio;

int main(int, char**)
{
  using namespace redi;

  EventManager manager;
  VanillaObserver obs;
  manager.addObserver(obs);
  manager.triggerWeatherChange();

  return 0;
}
