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
                struct  context
                {
                    thread_proc proc;
                    void       *data;
                };
                static handle launch(context &ctx, ID &tid );
                static void   finish(handle &h) throw();
                static handle get_current_handle();
                static ID     get_current_id();
                static bool   equal( const ID &lhs, const ID &rhs ) throw();
            };
        }

        //! one low level thread wrapper
        class thread
        {
        public:

#if defined(Y_BSD)
            //! pthread
            typedef pthread_t handle_t;
            typedef pthread_t id_t;
#define Y_THREAD_DECL(BODY) void * BODY entry( void *args ) throw()
#endif

#if defined(Y_WIN)
            //! win32
            typedef HANDLE handle_t;
            typedef DWORD  id_t;
#define Y_THREAD_DECL(BODY) DWORD WINAPI BODY entry( LPVOID args ) throw()
#endif
            typedef void (*procedure)(void*);



            thread     *next;   //!< for threads
            thread     *prev;   //!< for threads
            handle_t    handle; //!< system level thread handle
            id_t        id;     //!< system level thread identifier

            explicit thread(procedure proc,void *data);
            virtual ~thread() throw();

            static handle_t get_current_handle();;
            static id_t     get_current_id();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(thread);
            static Y_THREAD_DECL();
            procedure proc_;
            void     *data_;
        };

    }

}

#endif

