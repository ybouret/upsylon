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
        //! system recursive mutex
        class mutex : public lockable
        {
        public:
            explicit mutex() throw(); //!< initialize
            virtual ~mutex() throw(); //!< release all
            
            virtual void lock() throw();     //!< lock mutex
            virtual void unlock() throw();   //!< unlock mutex
            virtual bool try_lock() throw(); //!< try lock mutex
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(mutex);
#if defined(Y_BSD)
            typedef pthread_mutex_t mutex_type;
#endif
            
#if defined(Y_WIN)
            typedef CRITICAL_SECTION mutex_type;
#endif
            mutex_type impl_;
            void clear() throw();
        };
        
    }
    
}


#endif

