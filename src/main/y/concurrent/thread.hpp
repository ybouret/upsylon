//! \file
#ifndef Y_CONCURRENT_THREAD_INCLUDED
#define Y_CONCURRENT_THREAD_INCLUDED 1

#include "y/concurrent/mutex.hpp"
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
            typedef size_t tag_t;
            typedef void (*procedure)(void*);



            thread     *next;   //!< for threads
            thread     *prev;   //!< for threads
            handle_t    handle; //!< system level thread handle
            id_t        id;     //!< system level thread identifier
            const tag_t tag;    //!< application thread tag

            explicit thread(procedure proc,
                            void     *data,
                            const size_t user_tag);
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

