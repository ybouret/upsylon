//! \file
#ifndef Y_CONCURRENT_THREAD_INCLUDED
#define Y_CONCURRENT_THREAD_INCLUDED 1

#include "y/concurrent/mutex.hpp"
#include "y/object.hpp"

namespace upsylon
{

    namespace concurrent
    {


        class thread : public object
        {
        public:
            typedef void (*procedure)(void*);

#if defined(Y_BSD)
            //! pthread
            typedef pthread_t handle_t;
            typedef pthread_t id_t;
#endif

#if defined(Y_WIN)
            //! win32
            typedef HANDLE handle_t;
            typedef DWORD  id_t;
#endif

            thread   *next;   //!< for threads
            thread   *prev;   //!< for threads
            mutex    &access; //!< shared access from threads
            handle_t  handle; //!< system level thread handle
            id_t      id;     //!< system level thread identifier

#if 0
            static thread * launch(procedure user_proc,
                                   void     *user_data,
                                   mutex    &access_ref);

            static void     finish( thread * &thr) throw();
#endif
            
            explicit thread(procedure user_proc,
                            void     *user_data,
                            mutex    &access_ref);
            virtual ~thread() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(thread);
            procedure proc;
            void     *data;

#if defined(Y_BSD)
            static void * entry( void * ) throw();
#endif

#if defined(Y_WIN)
            static DWORD WINAPI entry( LPVOID ) throw();
#endif

        };

    }

}

#endif

