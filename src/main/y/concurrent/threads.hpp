//! \file
#ifndef Y_CONCURRENT_THREADS_INCLUDED
#define Y_CONCURRENT_THREADS_INCLUDED 1

#include "y/concurrent/thread.hpp"
#include "y/concurrent/condition.hpp"
#include "y/sequence/slots.hpp"
#include "y/concurrent/layout.hpp"
#include "y/ptr/auto.hpp"

namespace upsylon {

    namespace concurrent
    {
        typedef auto_ptr<const layout>       __topology;  //!< topology for threads
        typedef slots<thread,memory::global> __threads;   //!< memory for threads

        //! base class to handle threads creation/destruction
        class threads : public __topology, public __threads
        {
        public:
            mutex                  access;      //!< for threads synchronisation
            condition              synchronize; //!< for threads create/delete \todo PROTECT
            const size_t           running;     //!< running threads

            //! construct threads
            explicit threads(const bool v=false);
            //! quit threads
            virtual ~threads() throw();
            //! what to do in loop, upon signal or broadcast
            virtual void run(parallel &ctx) throw();
            //! wait for all threads to go back to waiting state
            void wait() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(threads);
            static void start(void*) throw();
            size_t      ready;   //!< to synchronize at ctor and dtor
            bool        dying;   //!< to break out of the loop when woke up
            void loop() throw(); //!< entry point method



        public:
            bool verbose; //!< verbose flag, mostly to debug

        };

        
    }

}

#endif

