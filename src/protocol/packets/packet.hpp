#ifndef REDI_PACKET_HPP
#define REDI_PACKET_HPP

#include "../../bytebuffer.hpp"
#include "../packetreader.hpp"
#include "../../session.hpp"

namespace redi
{

struct Packet
{
  virtual ~Packet() = 0;
  
  virtual void read(const ByteBuffer&) {}
  virtual void write(ByteBuffer&) {}
  
  virtual const char* getName() const { return ""; }
  
  void send(Session& session);
};

} // namespace redi

#endif // REDI_PACKET_HPP