
#include "y/concurrent/nucleus/thread.hpp"
#include "y/type/block/zset.hpp"
#include "y/exceptions.hpp"
#include "y/os/error.hpp"

namespace upsylon
{

    namespace concurrent
    {

        namespace  nucleus
        {
           

            void thread::finish(handle &h) throw()
            {
#if defined(Y_BSD)
                const int res = pthread_join( h, 0 );
                if( res != 0 )
                {
                    libc::critical_error( res, "pthread_join" );
                }

#endif

#if defined(Y_WIN)
                Y_GIANT_LOCK();
                if( ::WaitForSingleObject( h , INFINITE ) != WAIT_OBJECT_0 )
                {
                    win32::critical_error( ::GetLastError(), "WaitForSingleObject" );
                }
                ::CloseHandle( h );
#endif
                bzset(h);
            }

            thread::handle thread::get_current_handle() throw()
            {
#if defined(Y_BSD)
                return pthread_self();
#endif

#if defined(Y_WIN)
                return ::GetCurrentThread();
#endif
            }

            thread::ID thread::get_current_id() throw()
            {
#if defined(Y_BSD)
                return pthread_self();
#endif

#if defined(Y_WIN)
                return ::GetCurrentThreadId();
#endif
            }

            bool thread::equal( const ID &lhs, const ID &rhs ) throw()
            {
#if defined(Y_BSD)
                return pthread_equal(lhs,rhs)!=0;
#endif
#if defined(Y_WIN)
                return lhs==rhs;
#endif
            }

        }


    }

}
