//! \file
#ifndef Y_CONCURRENT_THREADS_INCLUDED
#define Y_CONCURRENT_THREADS_INCLUDED 1

#include "y/concurrent/executor.hpp"
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
        class threads : public executor, public __topology//, public __threads
        {
        public:
            mutable mutex access;      //!< for threads synchronisation


            virtual ~threads() throw();           //!< quit threads
            explicit threads(const bool v=false); //!< construct threads, waiting on start when ready

            virtual void       run(kernel code, void *data);      //!< unleash threads
            virtual size_t     num_threads() const throw();       //!< access parallelism
            virtual parallel & get_context(const size_t) throw(); //!< access contexts


        private:
            Y_DISABLE_COPY_AND_ASSIGN(threads);
            static  void system_entry(void*) throw(); //!< for system call, call this->thread_entry()
            void         thread_entry()      throw(); //!< parallel entry point

            __threads  engines;
            bool       halting;
            size_t     ready;
            condition  start;
            kernel     kproc;
            void      *kdata;

        public:
            bool verbose; //!< verbose flag, mostly to debug

        };

        
    }

}

#endif

