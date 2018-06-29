#ifndef Y_CONCURRENT_THREADS_INCLUDED
#define Y_CONCURRENT_THREADS_INCLUDED 1

#include "y/concurrent/thread.hpp"
#include "y/core/list.hpp"

namespace upsylon {

    namespace concurrent
    {

        class threads : public core::list_of_cpp<thread>
        {
        public:
            mutex access;

            explicit threads() throw();
            virtual ~threads() throw();

            inline void launch( thread::procedure proc, void *data)
            {
                push_back( new thread(proc,data,access) );
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(threads);
        };
    }

}

#endif

