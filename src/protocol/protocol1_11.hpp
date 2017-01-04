#ifndef REDI_PROTOCOL111_HPP
#define REDI_PROTOCOL111_HPP

#include "protocol.hpp"

namespace redi
{

class Protocol1_11 : public Protocol
{
public:

  Protocol1_11(Player& player) : Protocol(player) {}
  
  void readPacket(const BinaryData& buffer);
  void readHandshake(const BinaryData& buffer, std::size_t& offset);
};
  
} // namespace redi

#endif //REDI_PROTOCOL111_HPP
