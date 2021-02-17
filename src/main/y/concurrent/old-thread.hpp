//! \file
#ifndef Y_CONCURRENT_THREAD_INCLUDED
#define Y_CONCURRENT_THREAD_INCLUDED 1

#include "y/concurrent/nucleus/thread.hpp"
#include "y/concurrent/context.hpp"
#include "y/parallel.hpp"

namespace upsylon
{

    namespace concurrent
    {
        //! user low-level concurrent procedure
        typedef void (*thread_proc)(void*);

        

        //! thread wrapper
        class old_thread : public parallel
        {
        public:
            //! launch a thread
            old_thread(thread_proc  user_proc,
                       void        *user_args,
                       const size_t user_size,
                       const size_t user_rank);
            //! wait for thread to return and release resources
            virtual ~old_thread() throw();
            
            thread_proc                   proc;   //!< the procedure
            void                         *args;   //!< its data
            const nucleus::thread::ID     id;     //!< thread ID
            const nucleus::thread::handle handle; //!< thread handle

            bool is_current() const throw();      //!< checking against get_current_id

        private:
            Y_DISABLE_COPY_AND_ASSIGN(old_thread);
        };

        
    }

}

#endif

