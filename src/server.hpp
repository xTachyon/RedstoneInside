#ifndef REDI_SERVER
#define REDI_SERVER

#include <list>
#include "session.hpp"
#include "player.hpp"

namespace redi
{

class Server
{
public:

  Server() : mEntityCount(0) {}

  std::int32_t getNewEntityID() { return mEntityCount++; }
  
private:
  
  using SessionList = std::list<Session>;
  using PlayerList = std::list<Player>;
  
  SessionList mPlayerlessList;

  std::int32_t mEntityCount;
};
  
} // namespace redi



#endif // REDI_SERVER