//! \file

#ifndef Y_CONCURRENT_NUCLEUS_MUTEX_INCLUDED
#define Y_CONCURRENT_NUCLEUS_MUTEX_INCLUDED 1

#include "y/lockable.hpp"
#include "y/code/round.hpp"

#if defined(Y_BSD)
#    if !defined(__USE_UNIX98)
#        define __USE_UNIX98
#    endif
#    include <pthread.h>
#endif // Y_BSD

#if defined(Y_WIN)
#    define WIN32_LEAN_AND_MEAN
#    include <windows.h>
#endif // Y_WIN

namespace upsylon
{

    namespace concurrent
    {

        namespace nucleus
        {
            //! system mutex operation
            struct mutex
            {
                //! system mutex type
#if defined(Y_BSD)
                typedef pthread_mutex_t type;
#endif

#if defined(Y_WIN)
                typedef CRITICAL_SECTION type;
#endif
                static void init(type     *m) throw(); //!< initialize as recursive
                static void quit(type     *m) throw(); //!< release all data
                static void lock(type     *m) throw(); //!< lock
                static void unlock(type   *m) throw(); //!< unlock
                static bool try_lock(type *m) throw(); //!< try lock
            };
        }
    }
}

#endif
