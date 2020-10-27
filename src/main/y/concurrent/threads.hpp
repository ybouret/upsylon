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

    namespace concurrent {

        //! environment variable to set/get verbosity
#define Y_THREADS_VERBOSITY "Y_THREADS_VERBOSITY"

        typedef auto_ptr<const layout>       __topology;  //!< topology for threads
        typedef slots<thread,memory::global> __threads;   //!< memory for threads

        //----------------------------------------------------------------------
        //
        //! base class to handle threads creation/destruction
        //
        //----------------------------------------------------------------------
        class threads : public executor
        {
        public:
            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------
            virtual ~threads() throw(); //!< quit threads
            explicit threads();         //!< construct threads, waiting on start when ready
            

            //------------------------------------------------------------------
            //
            // kernel executor interface
            //
            //------------------------------------------------------------------
            virtual void            run(kernel code, void *data);             //!< unleash threads
            virtual size_t          size()                    const throw();  //!< access parallelism
            virtual parallel       & operator[](const size_t)       throw();  //!< access context in [1..size()]
            virtual const parallel & operator[](const size_t) const throw();  //!< access context in [1..size()], const

            //------------------------------------------------------------------
            //
            // auxiliary
            //
            //------------------------------------------------------------------
            static bool get_verbosity(); //!< get Y_THREADS_VERBOSITY environment variable

            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            const   __topology topology;    //!< local threads layout
            mutable mutex      access;      //!< for threads synchronisation

        private:
            Y_DISABLE_COPY_AND_ASSIGN(threads);
            static void        system_entry(void*) throw();       //!< for system call, call this->thread_entry()
            void               thread_entry()      throw();       //!< parallel entry point
            void               initialize();                      //!< prepare
            
            __threads  engines;
            bool       halting;
            size_t     ready;
            condition  start;
            kernel     kproc;
            void      *kdata;
            thread    *shift; //!< for parallel[1..size]

        public:
            bool verbose; //!< verbose flag, mostly to debug

        };

        
    }

}

#endif

