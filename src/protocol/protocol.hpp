#ifndef REDI_PROTOCOL_HPP
#define REDI_PROTOCOL_HPP

#include "../player.hpp"

namespace redi
{

class Protocol
{
public:
  
  Protocol(Player& player) : mPlayer(player) {}
  
  virtual ~Protocol() = 0;
  
protected:

  Player& mPlayer;
};

} // namespace redi

#endif // REDI_PROTOCOL_HPP
