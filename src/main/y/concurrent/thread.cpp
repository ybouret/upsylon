
#include "y/concurrent/thread.hpp"
#include "y/object.hpp"
#include "y/exceptions.hpp"
#include "y/os/error.hpp"
#include <cstring>
#include <iostream>

namespace upsylon
{

    namespace concurrent
    {

        thread:: thread(procedure proc,
                        void     *data,
                        const size_t user_tag):
        next(0),
        prev(0),
        handle(),
        id(),
        tag(user_tag),
        proc_(proc),
        data_(data)
        {
            std::cerr << "creating thread@tag=" << tag << std::endl;
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
            thr.proc_(thr.data_);
            return 0;
        }
        

        thread:: ~thread() throw()
        {
            std::cerr << "waiting for thread@tag=" << tag << std::endl;
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

