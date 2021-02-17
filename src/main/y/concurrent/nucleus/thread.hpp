
//! \file

#ifndef Y_CONCURRENT_NUCLEUS_INCLUDED
#define Y_CONCURRENT_NUCLEUS_INCLUDED 1

#include "y/concurrent/mutex.hpp"

namespace upsylon
{

    namespace concurrent
    {
        
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

                //typedef Y_THREAD_LAUNCHER_RETURN (*procedure)(Y_THREAD_LAUNCHER_PARAMS);
				//typedef DWORD (*WINAPI procedure)(LPVOID);
				typedef LPTHREAD_START_ROUTINE procedure;

                //! T.proc and T.data
                template <typename T>
                Y_THREAD_LAUNCHER_RETURN callback(Y_THREAD_LAUNCHER_PARAMS args) throw()
                {
                    assert(args);
                    T &caller = *static_cast<T*>(args); assert(caller.proc!=0);
                    caller.proc(caller.data);
                    return 0;
                }

                //! low level launch, info points at a concurrent::thread
                static handle launch_thread(void *info, ID &tid );

                //! low level launch, info points at a concurrent::worker
                static handle launch_worker(void *info, ID &tid );

				static handle launch(procedure proc, void *info, ID &tid);
                

                //! low level finish
                static void   finish(handle &h) throw();
                //! query system current thread handle
                static handle get_current_handle() throw();
                //! query system current thread ID
                static ID     get_current_id() throw();
                //! test two system thread IDs
                static bool   equal( const ID &lhs, const ID &rhs ) throw();
                //! try to bind handle to a given cpu
                static void   assign(handle       h,
                                     const size_t cpu,
                                     const char  *who);
                //! display info per cpu
                static void   display(const char *who, const size_t);
            };
            
        }

    }
}


#endif

