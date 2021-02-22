
//! \file

#ifndef Y_CONCURRENT_NUCLEUS_INCLUDED
#define Y_CONCURRENT_NUCLEUS_INCLUDED 1

#include "y/concurrent/nucleus/mutex.hpp"

namespace upsylon
{

    namespace concurrent
    {
        
        namespace nucleus
        {

            //__________________________________________________________________
            //
            //
            //! wraps low-level system calls
            //
            //__________________________________________________________________
            struct thread
            {
#if defined(Y_BSD)
                typedef pthread_t handle;                 //!< system thread handle
                typedef pthread_t ID;                     //!< system thread identifier
                typedef void *  (*system_routine)(void*); //!< system threadable interface
#define         Y_THREAD_LAUNCHER_RETURN void *
#define         Y_THREAD_LAUNCHER_PARAMS void *
#endif

#if defined(Y_WIN)
                typedef HANDLE handle;                         //!< system thread handle
                typedef DWORD  ID;                             //!< system thread identifier
                typedef LPTHREAD_START_ROUTINE system_routine; //!< system threadable interface
#define         Y_THREAD_LAUNCHER_RETURN DWORD WINAPI
#define         Y_THREAD_LAUNCHER_PARAMS LPVOID
#endif

                typedef void (*call)(void*); //!< portable function to call

                
                //! low level to start threaded code(data)
				static handle launch(system_routine code, void *data, ID &tid);

                //! start a thread with obj.proc(obj.args)
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

                //! check environment verbosity
                static bool   verbosity(const char *id);

                //! efficiency
                static double efficiency(const double speed_up,
                                         const size_t num_cpus) throw();

            private:
                //! T.proc and T.args
                template <typename T> static inline
                Y_THREAD_LAUNCHER_RETURN stub(Y_THREAD_LAUNCHER_PARAMS addr) throw()
                {
                    assert(addr);
                    T        &caller = *static_cast<T*>(addr);
                    call      proc   = caller.proc; assert(proc);
                    void     *args   = caller.args;
                    proc(args);
                    return 0;
                }
            };
            
        }

    }
}


#endif

