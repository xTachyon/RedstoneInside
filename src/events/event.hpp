#ifndef REDI_EVENT_EVENT
#define REDI_EVENT_EVENT

#include "eventtype.hpp"

namespace redi
{

class Event
{
public:

  void a() {}

private:
  
  EventType mType;
};

} // namespace redi

#endif //REDI_EVENT_EVENT
