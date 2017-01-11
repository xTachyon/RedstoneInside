#ifndef REDI_EVENT_HPP
#define REDI_EVENT_HPP

namespace redi
{

enum class EventType
{
  PlayerDC,
  SessionDC,
  SendKeepAlive
};

class Player;

class Event
{
public:
  
  Event(EventType type) : type_(type) {}
  
  virtual ~Event() = 0;
  
  EventType getType() const { return type_; }
  
  template <typename T>
  T& get()
  {
    T* ptr = dynamic_cast<T*>(this);
    if (ptr) return *ptr;
    throw std::runtime_error("dynamic cast failed");
  }
  
private:
  
  EventType type_;
};

inline Event::~Event() {}

using EventPtr = std::shared_ptr<Event>;

class EventPlayerDC : public Event
{
public:
  
  Player* player;
  
  EventPlayerDC(Player* player) : Event(EventType::PlayerDC), player(player) {}
  
};

class EventSessionDC : public Event
{
public:
  
  Session* session;
  
  EventSessionDC(Session* s) : Event(EventType::SessionDC), session(s) {}
};

struct EventSendKeepAliveRing : public Event
{
  Session& session;
  
  EventSendKeepAliveRing(Session& session) : Event(EventType::SendKeepAlive), session(session) {}
};

} // namespace redi

#endif //REDI_EVENT_HPP
