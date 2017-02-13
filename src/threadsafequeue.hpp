#ifndef REDI_THREADSAFEQUEUE
#define REDI_THREADSAFEQUEUE

#include <deque>
#include <mutex>

namespace redi
{

template <typename T>
class ThreadSafeQueue
{
public:
  
  ThreadSafeQueue() = default;
  
  void push(const T& obj)
  {
    std::lock_guard<std::mutex> l(mUtex);
    mData.push_back(obj);
  }
  
  void push(T&& obj)
  {
    std::lock_guard<std::mutex> l(mUtex);
    mData.push_back(std::move(obj));
  }
  
  T pop()
  {
    std::lock_guard<std::mutex> l(mUtex);
    if (mData.empty()) return {};
    T ret(std::move(*mData.begin()));
    mData.pop_front();
    return ret;
  }
  
  bool empty() const { return mData.empty(); }
  
private:
  
  std::deque<T> mData;
  std::mutex mUtex;
};

} // namespace redi

#endif // REDI_THREADSAFEQUEUE