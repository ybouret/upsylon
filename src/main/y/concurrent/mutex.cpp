#include "y/concurrent/mutex.hpp"
#include "y/os/error.hpp"
#include "y/type/utils.hpp"

#include <cstring>
#include <iostream>

namespace upsylon
{
    namespace concurrent
    {
        
        mutex:: ~mutex() throw()
        {
#if defined(Y_WIN)
            ::DeleteCriticalSection( & impl_ );
#endif
            
#if defined(Y_BSD)
            {
                const int res = pthread_mutex_destroy( &impl_ );
                if( res != 0 )
                    libc::critical_error( res, "pthread_mutex_destroy" );
            }
#endif
            clear();
        }
        
        mutex:: mutex() throw():
        impl_()
        {
            clear();
#if defined(Y_WIN)
            ::InitializeCriticalSection( & mutex_ );
#endif
            
#if defined(Y_BSD)
            {
                pthread_mutexattr_t attr;
                
                int  res = pthread_mutexattr_init( &attr );
                if (res != 0)
                    libc::critical_error(res, "pthread_mutexattr_init");
                
                res = pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_RECURSIVE);
                if (res != 0)
                {
                    pthread_mutexattr_destroy( &attr );
                    libc::critical_error(res, "pthread_mutexattr_settype(RECURSIVE)");
                }
                
                res = pthread_mutex_init( & impl_ , &attr );
                (void) pthread_mutexattr_destroy( &attr );
                if( res != 0 )
                    libc::critical_error(res,"pthread_mutex_init");
            }
#endif
            
        }
        
        void mutex:: clear() throw()
        {
            memset( &impl_, 0, sizeof(impl_) );
        }
        
        void mutex:: lock() throw()
        {
#if defined(Y_WIN)
            ::EnterCriticalSection(  & impl_  );
#endif
            
#if defined(Y_BSD)
            const int res = pthread_mutex_lock( &impl_ );
            if( res != 0 )
                libc::critical_error( res,  "pthread_mutex_lock" );
#endif
        }
        
        void mutex:: unlock() throw()
        {
#if defined(Y_WIN)
            ::LeaveCriticalSection( &impl_  );
#endif
            
#if defined(Y_BSD)
            const int res = pthread_mutex_unlock( & impl_ );
            if( res != 0 )
                libc::critical_error( res,  "pthread_mutex_lock" );
#endif
        }
        
        bool mutex:: try_lock() throw()
        {
#if defined(Y_WIN)
            return TryEnterCriticalSection( &impl_ ) == TRUE;
#endif
            
#if defined(Y_BSD)
            return pthread_mutex_trylock( &impl_ ) == 0;
#endif
            
        }
        
    }
}

