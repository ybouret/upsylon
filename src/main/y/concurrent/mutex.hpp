//! \file
#ifndef Y_CONCURRENT_MUTEX_INCLUDED
#define Y_CONCURRENT_MUTEX_INCLUDED 1

#include "y/lockable.hpp"

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
                static void init( type *m )   throw(); //!< initialize as recursive
                static void quit(type *m)     throw(); //!< release all data
                static void lock(type *m)     throw(); //!< lock
                static void unlock(type *m)   throw(); //!< unlock
                static bool try_lock(type *m) throw(); //!< try lock
            };
        }

        //! system recursive mutex
        class mutex : public lockable
        {
        public:
            //! create the system mutex
            inline explicit mutex() throw() : m()  { nucleus::mutex::init(&m);            }
            //! release all
            inline virtual ~mutex() throw()        { nucleus::mutex::quit(&m);            }
            //! lock mutex
            inline virtual void lock()     throw() { nucleus::mutex::lock(&m);            }
            //! unlock mutex
            inline virtual void unlock()   throw() { nucleus::mutex::unlock(&m);          }
            //! try lock mutex
            inline virtual bool try_lock() throw() { return nucleus::mutex::try_lock(&m); }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(mutex);
            nucleus::mutex::type m;

        public:
            static mutex giant;
        };

#define Y_GIANT_LOCK() Y_LOCK(concurrent::mutex::giant)
    }
    
}


#endif

