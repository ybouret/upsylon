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
        
        class mutex : public lockable
        {
        public:
            explicit mutex(const char * = 0) throw();
            virtual ~mutex() throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(mutex);
#if defined(Y_BSD)
            pthread_mutex_t impl_;
#endif
            
#if defined(Y_WIN)
            CRITICAL_SECTION impl_;
#endif
            
        };
        
    }
    
}


#endif

