
#include "y/concurrent/thread.hpp"
#include "y/object.hpp"
#include "y/exceptions.hpp"
#include "y/os/error.hpp"
#include <cstring>

namespace upsylon
{

    namespace concurrent
    {

        thread:: thread(procedure user_proc,
                        void     *user_data,
                        mutex    &access_ref):
        next(0),
        prev(0),
        access(access_ref),
        handle(),
        id(),
        proc(user_proc),
        data(user_data)
        {
            memset(&handle,0,sizeof(handle));
            memset(&id,0,sizeof(id));

#if    defined(Y_BSD)
            //------------------------------------------------------------------
            // <pthread>
            //------------------------------------------------------------------
            const int res = pthread_create( &handle, NULL, thread::entry, this);
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
        

#if defined(Y_BSD)
        void * thread::entry(void *args) throw()
        {
            thread *thr= static_cast<thread *>(args);
            assert( thr );
            thr->proc( thr->data );
            return 0;
        }
#endif

#if defined(Y_WIN)
        DWORD WINAPI thread::entry( LPVOID args ) throw()
        {
            thread *thr = static_cast<thread *>(args);
            assert( thr );
            thr->proc( thr->data );
            return 0;
        }

#endif


        thread:: ~thread() throw()
        {
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

