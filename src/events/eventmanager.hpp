#ifndef REDI_EVENT_EVENTMANAGER
#define REDI_EVENT_EVENTMANAGER

#include <functional>
#include <deque>
#include <memory>
#include "observer.hpp"

namespace redi
{

struct FunctorBase
{
  virtual ~FunctorBase() {}
};

template <typename T>
struct Functor : public FunctorBase
{
  std::function<void(T&)> callback;

  void operator()(T& arg)
  {
    callback(arg);
  }
};

class EventManager
{
public:
  
  
  
private:
  
  std::deque<std::unique_ptr<FunctorBase>> mEventQueue;
};
  
} // namespace redi

#endif // REDI_EVENT_EVENTMANAGER