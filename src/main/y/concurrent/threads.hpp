#ifndef Y_CONCURRENT_THREADS_INCLUDED
#define Y_CONCURRENT_THREADS_INCLUDED 1

#include "y/concurrent/thread.hpp"
#include "y/concurrent/condition.hpp"
#include "y/core/list.hpp"

namespace upsylon {

    namespace concurrent
    {

        //! lowe level threads
        class threads : public core::list_of_cpp<thread>
        {
        public:
            mutex     access;
            condition sync;

            explicit threads() throw();
            virtual ~threads() throw();

            inline void launch( thread::procedure user_proc, void *user_data)
            {
                assert(user_proc);
                push_back( new thread(user_proc,user_data) );
            }

            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(threads);
        };
    }

}

#endif

