
//! \file

#ifndef Y_CONCURRENT_NUCLEUS_INCLUDED
#define Y_CONCURRENT_NUCLEUS_INCLUDED 1

#include "y/concurrent/mutex.hpp"

namespace upsylon
{

    namespace concurrent
    {
        //! user low-level concurrent procedure
        typedef void (*thread_proc)(void*);

        namespace nucleus
        {
            //! wraps low-level system calls
            struct thread
            {
#if defined(Y_BSD)
                typedef pthread_t handle;
                typedef pthread_t ID;
#define         Y_THREAD_LAUNCHER_RETURN void *
#define         Y_THREAD_LAUNCHER_PARAMS void *
#endif

#if defined(Y_WIN)
                typedef HANDLE handle;
                typedef DWORD  ID;
#define         Y_THREAD_LAUNCHER_RETURN DWORD WINAPI
#define         Y_THREAD_LAUNCHER_PARAMS LPVOID
#endif

                //! low level launch, info points at a concurrent::thread
                static handle launch(void *info, ID &tid );
                //! low level finish
                static void   finish(handle &h) throw();
                //! query system current thread handle
                static handle get_current_handle() throw();
                //! query system current thread ID
                static ID     get_current_id() throw();
                //! test two system thread IDs
                static bool   equal( const ID &lhs, const ID &rhs ) throw();
                //! try to bind handle to a given cpu
                static void   assign( handle h, const size_t cpu );
            };
            
        }

    }
}


#endif

