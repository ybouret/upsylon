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
        class threads : public executor, public __topology, public __threads
        {
        public:
            mutex access;      //!< for threads synchronisation

            //! quit threads
            virtual ~threads() throw();
            //! construct threads
            explicit threads(const bool v=false);

            //! unleashed threads
            virtual void run(kernel code, void *data);

            //! access parallelism
            virtual size_t num_threads() const throw() { return this->size(); }

            //! access contexts
            inline virtual parallel & operator[](const size_t indx) throw()
            {
                __threads &self = *this; return self[indx];
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(threads);
            static  void system_entry(void*) throw(); //!< for system call, call this->thread_entry()
            void         thread_entry()      throw(); //!< parallel entry point
            
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

