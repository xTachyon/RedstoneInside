#ifndef REDI_LOCKFREE_QUEUE
#define REDI_LOCKFREE_QUEUE

#include <deque>
#include <mutex>
#include <boost/optional.hpp>

namespace redi
{
namespace lockfree
{

template <typename T>
class Queue
{
public:
  
  using Type = T;
  using OptionalType = boost::optional<T>;
  
  ~Queue()
  {
    std::lock_guard<std::mutex> l(mMutex);
  }
  
  void push(const T& obj)
  {
    std::lock_guard<std::mutex> l(mMutex);
    
    mData.push_back(obj);
  }
  
  void push(T&& obj)
  {
    std::lock_guard<std::mutex> l(mMutex);
    
    mData.push_back(std::move(obj));
  }
  
  bool pop(T& obj)
  {
    std::lock_guard<std::mutex> l(mMutex);
    
    if (mData.empty())
    {
      return false;
    }
    else
    {
      obj = std::move(mData.front());
      mData.pop_front();
      return true;
    }
  }
  
  OptionalType pop()
  {
    std::lock_guard<std::mutex> l(mMutex);
    OptionalType result;
  
    if (!mData.empty())
    {
      result = std::move(mData.front());
      mData.pop_front();
    }
  
    return result;
  }
  
private:
  
  std::deque<T> mData;
  std::mutex mMutex;
};

using ByteBufferQueue = Queue<ByteBuffer>;
  
} // namespace lockfree
} // namespace redi

#endif // REDI_LOCKFREE_QUEUE