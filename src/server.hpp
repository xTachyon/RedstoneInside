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
  
private:
  
  using SessionList = std::list<Session>;
  using PlayerList = std::list<Player>;
  
  SessionList mPlayerlessList;
};
  
} // namespace redi



#endif // REDI_SERVER