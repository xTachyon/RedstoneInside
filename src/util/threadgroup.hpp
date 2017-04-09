#ifndef REDI_UTIL_THREADGROUP_HPP
#define REDI_UTIL_THREADGROUP_HPP

#include <thread>
#include <vector>

namespace redi {
namespace util {

template <typename Thread = std::thread>
struct ThreadGroup {
  using Container = std::vector<Thread>;
  
  Container threads;
  
  ThreadGroup() = default;
  
  template <typename Callable, typename ... Args>
  ThreadGroup(int number, Callable&& f, Args ... args) {
    create(number, std::forward<Callable>(f),
           std::forward<Args>(args) ...);
  }
  
  ~ThreadGroup() {
    joinAll();
  }
  
  template <typename Callable, typename ... Args>
  void create(int number, Callable&& f, Args ... args) {
    for (int i = 0; i < number; ++i) {
      threads.emplace_back(std::forward<Callable>(f),
                           std::forward<Args>(args) ...);
    }
  }
  
  void joinAll() {
    for (auto& i : threads) {
      if (i.joinable()) {
        i.join();
      }
    }
  }
};

}
}

#endif // REDI_UTIL_THREADGROUP_HPP