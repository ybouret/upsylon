
#include "y/concurrent/thread.hpp"
#include "y/exceptions.hpp"
#include "y/os/error.hpp"
#include "y/type/bzset.hpp"

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

                return 0;
            }



            thread::handle thread::launch( context &ctx, ID &tid)
            {
                assert(ctx.proc);
                bzset(tid);
#if    defined(Y_BSD)
                const int res = pthread_create( &tid, NULL, thread_launcher, &ctx);
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
                                          &ctx,
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
            }

            thread::handle thread::get_current_handle()
            {
#if defined(Y_BSD)
                return pthread_self();
#endif

#if defined(Y_WIN)
                return ::GetCurrentThread();
#endif
            }

            thread::ID thread::get_current_id()
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

        thread:: thread(procedure proc, void *data):
        next(0),
        prev(0),
        handle(),
        id(),
        proc_(proc),
        data_(data)
        {
            assert(proc||die("need a thread procedure"));
            memset(&handle,0,sizeof(handle));
            memset(&id,0,sizeof(id));

#if    defined(Y_BSD)
            //------------------------------------------------------------------
            // <pthread>
            //------------------------------------------------------------------
            const int res = pthread_create( &handle, NULL, thread::entry,this);
            if( res != 0 )
            {
                throw libc::exception( res, "pthread_create" );
            }
            id=handle;
            //------------------------------------------------------------------
            // <pthread/>
            //------------------------------------------------------------------
#endif

#if defined(Y_WIN)
            //------------------------------------------------------------------
            // <win32>
            //------------------------------------------------------------------
            Y_GIANT_LOCK();
            handle = ::CreateThread(0 ,
                                    0 ,
                                    thread::entry,
                                    this,
                                    0,
                                   &id );
            if( NULL == handle )
            {
                const DWORD res = ::GetLastError();
                throw win32::exception( res, "::CreateThread" );
            }
            //------------------------------------------------------------------
            // <win32/>
            //------------------------------------------------------------------
#endif
        }
        

        Y_THREAD_DECL(thread::)
        {
            assert(args);
            thread &thr = *static_cast<thread *>(args);
            assert(thr.proc_);
            thr.proc_( thr.data_ );
            return 0;
        }
        

        thread:: ~thread() throw()
        {
            assert(!next);
            assert(!prev);
#if defined(Y_BSD)
            const int res = pthread_join( handle, 0 );
            if( res != 0 )
            {
                libc::critical_error( res, "pthread_join" );
            }

#endif

#if defined(Y_WIN)
            Y_GIANT_LOCK();
            if( ::WaitForSingleObject( handle , INFINITE ) != WAIT_OBJECT_0 )
            {
                win32::critical_error( ::GetLastError(), "WaitForSingleObject" );
            }
            ::CloseHandle( handle );
#endif

        }


    }

}

