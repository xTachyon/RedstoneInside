#ifndef REDI_MINGWWORKAROUND
# define REDI_MINGWWORKAROUND

# if defined(__MINGW32__) || defined(__MINGW64__)

#  include <boost/thread.hpp>

namespace std
{
    using boost::mutex;
    //using boost::recursive_mutex;
    //using boost::lock_guard;
    //using boost::condition_variable;
    //using boost::unique_lock;
    using boost::thread;
}
#  endif

#endif //REDI_MINGWWORKAROUND
