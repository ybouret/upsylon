#ifndef Y_CONCURRENT_THREADS_INCLUDED
#define Y_CONCURRENT_THREADS_INCLUDED 1

#include "y/concurrent/thread.hpp"
#include "y/concurrent/condition.hpp"
#include "y/sequence/slots.hpp"

namespace upsylon {

    namespace concurrent
    {
        typedef slots<thread,memory::global> __threads;

        class threads : public __threads
        {
        public:
            mutex     access;
            condition synchronize;
            
            explicit threads(size_t n, const bool v=false);
            virtual ~threads() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(threads);
            static void start(void*) throw();
            size_t      ready;   //!< to synchronize at ctor and dtor
            bool        dying;   //!< to break out of the loop when woke up
            void loop() throw();

        public:
            bool verbose;

        };

        
    }

}

#endif

