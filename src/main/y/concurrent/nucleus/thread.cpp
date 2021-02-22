
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
            
            thread::handle thread::launch(system_routine code, void *data, ID &tid)
            {
                assert(code);
                assert(data);
                bzset(tid);
#if    defined(Y_BSD)
                const int res = pthread_create(&tid, NULL, code, data);
                if (res != 0)
                {
                    throw libc::exception(res, "pthread_create");
                }
                return tid;
#endif

#if defined(Y_WIN)
                handle h = ::CreateThread(0,
                                          0,
                                          code,
                                          data,
                                          0,
                                          &tid);
                if (NULL == h)
                {
                    const DWORD res = ::GetLastError();
                    throw win32::exception(res, "::CreateThread");
                }
                return h;
#endif
            }



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
