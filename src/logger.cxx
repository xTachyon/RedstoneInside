#include <iostream>
#include <mutex>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include "logger.hpp"

namespace fs = boost::filesystem;

namespace redi
{

Logger::Logger()
  : mStop(false)
{
  fs::create_directories("logs");
  mFile.open("logs/latest.txt", std::ios::app);
  mThread = std::thread(&Logger::workingThread, this);
}

Logger::~Logger()
{
  mStop = true;
  mThread.join();
}

void Logger::workingThread()
{
//  using namespace std::chrono_literals;

  Container local;
  std::string result;

  while (!mStop)
  {
    local.clear();
    {
      std::lock_guard<std::mutex> guard(mMutex);
      local.swap(mQueue);
    }

    run(local, result);
    boost::this_thread::sleep_for(boost::chrono::milliseconds(5));
        //(5ms);
  }

  run(mQueue, result);
}

void Logger::run(Container& cont, std::string& result)
{
  result.clear();

  for (const auto& i : cont)
    result += (boost::format("[%2% - %3%]: %1%\n") % std::get<0>(i) % std::get<1>(i) % getEnumName(std::get<2>(i))).str();

  if (result.size() != 0)
  {
    std::cout << result;
    mFile << result << std::flush;
  }
}

const char* Logger::getEnumName(LoggerLevel level)
{
  const char* ptr;

  switch (level)
  {
  case LoggerLevel::Debug:
    ptr = "debug";
    break;

  case LoggerLevel::Error:
    ptr = "error";
    break;

  case LoggerLevel::Fatal:
    ptr = "fatal";
    break;

  case LoggerLevel::Info:
    ptr = "info";
    break;

  case LoggerLevel::Warn:
    ptr = "warn";
    break;

  default:
    ptr = "unknown";
  }

  return ptr;
}

Logger& Logger::get()
{
  static Logger log;
  return log;
}

} // namespace redi