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
            mutex access;      //!< for threads synchronisation

            //! quit threads
            virtual ~threads() throw();

        protected:
            //! construct threads
            explicit threads(const bool v=false);
            bool     halting;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(threads);
            static  void entry(void*) throw();
            size_t     ready;
            condition  start;
            
            void start_thread() throw();


        public:
            bool verbose; //!< verbose flag, mostly to debug

        };

        
    }

}

#endif

