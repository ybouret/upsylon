//! \file
#ifndef Y_THREAD_WORKER_INCLUDED
#define Y_THREAD_WORKER_INCLUDED 1

#include "y/concurrent/nucleus/thread.hpp"
#include "y/concurrent/context.hpp"

namespace upsylon {

    namespace concurrent
    {

        //______________________________________________________________________
        //
        //
        //! parallel execution of persistent proc/data
        //
        //______________________________________________________________________
        class worker : public context
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            typedef void (*call)(void *); //!< to run within a thread

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            //! execute proc(data) within a new thread
            worker(call         user_proc,
                   void        *user_data,
                   const size_t user_size,
                   const size_t user_rank);

            //! wait for thread to return and release resources
            virtual ~worker() throw();

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            call                          proc;   //!< the procedure
            void                         *data;   //!< its data
            const nucleus::thread::ID     id;     //!< thread ID
            const nucleus::thread::handle handle; //!< thread handle

            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(worker);
        };


    }

}

#endif
