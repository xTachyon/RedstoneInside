#ifndef REDI_LOCKFREE_QUEUE
#define REDI_LOCKFREE_QUEUE

#include <deque>
#include <mutex>
#include <boost/optional.hpp>
#include "../bytebuffer.hpp"

namespace redi {
namespace lockfree {

template <typename T>
class Queue {
public:
  using Type = T;
  using OptionalType = boost::optional<T>;
  using Container = std::deque<T>;

  ~Queue() { std::lock_guard<std::mutex> l(mutex); }

  void push(const T& obj) {
    std::lock_guard<std::mutex> l(mutex);

    data.push_back(obj);
  }

  void push(T&& obj) {
    std::lock_guard<std::mutex> l(mutex);

    data.push_back(std::move(obj));
  }

  void push(T&& first, T&& second) {
    std::lock_guard<std::mutex> l(mutex);

    data.push_back(std::move(first));
    data.push_back(std::move(second));
  }

  bool pop(T& obj) {
    std::lock_guard<std::mutex> l(mutex);

    if (data.empty()) {
      return false;
    } else {
      obj = std::move(data.front());
      data.pop_front();
      return true;
    }
  }

  OptionalType pop() {
    std::lock_guard<std::mutex> l(mutex);
    OptionalType result;

    if (!data.empty()) {
      result = std::move(data.front());
      data.pop_front();
    }

    return result;
  }
  
  void swap(Container& cont) {
    std::lock_guard<std::mutex> l(mutex);
    
    cont.swap(data);
  }
  
  std::mutex& getMutex() { return mutex; }
  
private:
  Container data;
  std::mutex mutex;
};

using ByteBufferQueue = Queue<ByteBuffer>;

} // namespace lockfree
} // namespace redi

#endif // REDI_LOCKFREE_QUEUE