#ifndef REDI_EVENTS_FUNCTOR_HPP
#define REDI_EVENTS_FUNCTOR_HPP

#include <functional>

namespace redi
{

struct FunctorBase
{
  virtual ~FunctorBase() = 0;
};

inline FunctorBase::~FunctorBase() {}

template <typename T>
struct Functor : FunctorBase
{
  std::function<T> f;
};

} // namespace redi

#endif // REDI_EVENTS_FUNCTOR_HPP