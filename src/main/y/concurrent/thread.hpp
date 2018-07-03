//! \file
#ifndef Y_CONCURRENT_THREAD_INCLUDED
#define Y_CONCURRENT_THREAD_INCLUDED 1

#include "y/concurrent/mutex.hpp"

namespace upsylon
{

    namespace concurrent
    {
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

                static handle launch(void *info, ID &tid );
                static void   finish(handle &h) throw();
                static handle get_current_handle() throw();
                static ID     get_current_id() throw();
                static bool   equal( const ID &lhs, const ID &rhs ) throw();
            };
        }

        //! thread wrapper
        class thread
        {
        public:
            thread(thread_proc user_proc,
                   void       *user_data);
            ~thread() throw();
            thread_proc                   proc;
            void                         *data;
            const nucleus::thread::ID     id;
            const nucleus::thread::handle handle;

            bool is_current() const throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(thread);
        };

        
    }

}

#endif

