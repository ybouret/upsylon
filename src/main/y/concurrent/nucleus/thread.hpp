
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
                typedef void *  (*routine)(void*);
#define         Y_THREAD_LAUNCHER_RETURN void *
#define         Y_THREAD_LAUNCHER_PARAMS void *
#endif

#if defined(Y_WIN)
                typedef HANDLE handle;
                typedef DWORD  ID;
                typedef LPTHREAD_START_ROUTINE routine;
#define         Y_THREAD_LAUNCHER_RETURN DWORD WINAPI
#define         Y_THREAD_LAUNCHER_PARAMS LPVOID
#endif
                typedef void (*call)(void*);

                //! low level launch, info points at a concurrent::thread
                static handle launch_thread(void *info, ID &tid );

                //! low level to start threaded code(data)
				static handle launch(routine code, void *data, ID &tid);

                template <typename T> static inline
                handle launch(T &obj, ID &tid)
                {
                    return launch( stub<T>, &obj, tid );
                }


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

            private:
                //! T.proc and T.args
                template <typename T> static inline
                Y_THREAD_LAUNCHER_RETURN stub(Y_THREAD_LAUNCHER_PARAMS addr) throw()
                {
                    assert(addr);
                    T &caller = *static_cast<T*>(addr);
                    call      proc = caller.proc; assert(proc);
                    void     *args = caller.args;
                    proc(args);
                    return 0;
                }
            };
            
        }

    }
}


#endif

