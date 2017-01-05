#ifndef REDI_PROTOCOL111_HPP
#define REDI_PROTOCOL111_HPP

#include "protocol.hpp"

namespace redi
{

class Protocol1_11 : public Protocol
{
public:

  Protocol1_11(Player& player) : Protocol(player) {}
  
  void readPacket(const ByteBuffer& buffer);
  void readHandshake(const ByteBuffer& buffer, std::size_t& offset);
};
  
} // namespace redi

#endif //REDI_PROTOCOL111_HPP
