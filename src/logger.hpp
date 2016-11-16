#ifndef REDI_LOGGER
# define REDI_LOGGER

# include <deque>
# include <fstream>
# include <mutex>
# include <thread>
# include <tuple>
# include <boost/date_time/posix_time/posix_time.hpp>
# include "mingwworkaround.hpp"

namespace redi
{

enum class LoggerLevel
{
  Debug,
  Info,
  Warn,
  Error,
  Fatal
};

class Logger
{
public:

  Logger();
  ~Logger();

  template<typename T>
  void write(const T& val, LoggerLevel level = LoggerLevel::Info)
  {
    std::lock_guard<std::mutex> guard(mMutex);
    mSstream.str("");
    mSstream << val;
    mQueue.push_back(std::make_tuple(mSstream.str(), boost::posix_time::second_clock::local_time(), level));
  }

  template<typename T>
  static void debug(const T& val)
  {
#ifdef _DEBUG
    get().write(val, LoggerLevel::Debug);
#endif
  }

  template<typename T>
  static void debugSync(const T& val)
  {
#ifdef _DEBUG
    std::cout << "DEBUGSYNC: " << val << '\n';
#endif
  }

  template<typename T>
  static void info(const T& val)
  {
    get().write(val, LoggerLevel::Info);
  }

  template<typename T>
  static void warn(const T& val)
  {
    get().write(val, LoggerLevel::Warn);
  }

  template<typename T>
  static void error(const T& val)
  {
    get().write(val, LoggerLevel::Error);
  }

  template<typename T>
  static void fatal(const T& val)
  {
    get().write(val, LoggerLevel::Fatal);
  }

private:

  using Container = std::deque<std::tuple<std::string, boost::posix_time::ptime, LoggerLevel>>;
  
  Container mQueue;
  std::thread mThread;
  std::ofstream mFile;
  std::mutex mMutex;
  std::ostringstream mSstream;
  bool mStop;

  void workingThread();
  void run(Container& cont, std::string& result);
  const char* getEnumName(LoggerLevel level);
  static Logger& get();
};

} // namespace redi

#endif // REDI_LOGGER