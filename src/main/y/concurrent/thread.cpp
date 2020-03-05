
#include "y/concurrent/thread.hpp"
#include "y/exceptions.hpp"
#include "y/os/error.hpp"
#include "y/type/block/zset.hpp"

#include <iostream>

namespace upsylon
{

    namespace concurrent
    {

        namespace  nucleus
        {
            Y_THREAD_LAUNCHER_RETURN thread_launcher( Y_THREAD_LAUNCHER_PARAMS args ) throw()
            {
                assert(args);
                concurrent::thread & thr = *static_cast<concurrent::thread*>(args);
                assert(thr.proc);
                thr.proc(thr.data);
                return 0;
            }



            thread::handle thread::launch( void *args, ID &tid)
            {
                assert(args);
                bzset(tid);
#if    defined(Y_BSD)
                const int res = pthread_create( &tid, NULL, thread_launcher,args);
                if( res != 0 )
                {
                    throw libc::exception( res, "pthread_create" );
                }
                return tid;
#endif

#if defined(Y_WIN)
                Y_GIANT_LOCK();
                handle h = ::CreateThread(0 ,
                                          0 ,
                                          thread_launcher,
                                          args,
                                          0,
                                          &tid );
                if( NULL == h )
                {
                    const DWORD res = ::GetLastError();
                    throw win32::exception( res, "::CreateThread" );
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

namespace upsylon
{

    namespace concurrent
    {
        thread:: thread(thread_proc  user_proc,
                        void        *user_data,
                        const size_t user_size,
                        const size_t user_rank) :
        parallel(user_size,user_rank),
        proc(user_proc),
        data(user_data),
        id(),
        handle( nucleus::thread::launch(this,(nucleus::thread::ID &)id) )
        {
        }

        thread:: ~thread() throw()
        {
            nucleus::thread::finish((nucleus::thread::handle&)handle);
            bzset( (nucleus::thread::ID &)id );
        }
    }
}


