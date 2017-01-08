#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include "time.hpp"

namespace redi
{
namespace util
{

std::int32_t getUnixTimestamp()
{
  boost::posix_time::ptime epoch(boost::gregorian::date(1970, 1, 1));
  return (boost::posix_time::second_clock::universal_time() - epoch).total_seconds();
}

std::int64_t getUnixTimestampMilliseconds()
{
  boost::posix_time::ptime epoch(boost::gregorian::date(1970, 1, 1));
  return (boost::posix_time::second_clock::universal_time() - epoch).total_milliseconds();
}
  
} // namespace util
} // namespace redi
