//! \file
#ifndef Y_CONCURRENT_THREAD_INCLUDED
#define Y_CONCURRENT_THREAD_INCLUDED 1

#include "y/concurrent/condition.hpp"
#include "y/object.hpp"

namespace upsylon
{

    namespace concurrent
    {


        //! one low level thread wrapper
        class thread : public object
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
            class setup {
            public:
                mutex      access;
                condition  start;
                procedure  proc;
                void      *data;
                inline setup() throw() : access(), start(), proc(), data() {}
                inline virtual ~setup() throw() {}
            private:
                Y_DISABLE_COPY_AND_ASSIGN(setup);
            };



            thread     *next;   //!< for threads
            thread     *prev;   //!< for threads
            handle_t    handle; //!< system level thread handle
            id_t        id;     //!< system level thread identifier

            explicit thread(setup &todo);
            virtual ~thread() throw();

            static handle_t get_current_handle();;
            static id_t     get_current_id();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(thread);
            static Y_THREAD_DECL();
            setup *ini;
        };

    }

}

#endif

