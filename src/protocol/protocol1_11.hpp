#ifndef REDI_PROTOCOL111_HPP
#define REDI_PROTOCOL111_HPP

#include "protocol.hpp"
#include "../bytebuffer.hpp"

namespace redi
{

class Protocol1_11 : public Protocol
{
public:
  
  Protocol1_11(Session* ptr) : Protocol(ptr) {}
  
  virtual ~Protocol1_11();
  
  virtual void handlePacket(ByteBuffer& buffer) override;
  virtual void handleHandshake(PacketReader& reader) override;
  virtual void handleStatusRequest(PacketReader& reader) override;
  virtual void handleStatusPing(PacketReader&) override;
  
  virtual void sendStatusPong(std::int64_t number) override;
  
};

inline Protocol1_11::~Protocol1_11() {}
  
} // namespace redi

#endif //REDI_PROTOCOL111_HPP
