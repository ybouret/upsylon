
//! \file

#ifndef Y_CONCURRENT_THREAD_INCLUDED
#define Y_CONCURRENT_THREAD_INCLUDED 1

#include "y/concurrent/context.hpp"
#include "y/concurrent/nucleus/thread.hpp"

namespace upsylon
{

    namespace concurrent
    {

        class thread : public context
        {
        public:
            explicit thread(nucleus::thread::call user_call,
                            void                 *user_args,
                            const size_t          user_size,
                            const size_t          user_rank);

            virtual ~thread() throw();
            

            nucleus::thread::call         proc;
            void                         *args;
            const nucleus::thread::ID     id;
            const nucleus::thread::handle handle;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(thread);
            
        };

    }
}

#endif

