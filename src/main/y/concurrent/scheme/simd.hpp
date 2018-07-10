//! \file
#ifndef Y_CONCURRENT_SIMD_INCLUDED
#define Y_CONCURRENT_SIMD_INCLUDED 1

#include "y/concurrent/scheme/for-each.hpp"
#include "y/concurrent/threads.hpp"

namespace upsylon
{
    namespace concurrent
    {
        //! Single Instruction Multiple Data
        class simd :  public for_each
        {
        public:
            //! construct threads with(out) verbosity flag
            explicit simd(const bool v=false);
            //! destructor
            virtual ~simd() throw();

            //! for_each interface: execute multiple copy of the kernel
            virtual void run( kernel code, void *data );

            //! the underlying threads
            virtual executor & engine() throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(simd);
            static  void call( void *, parallel &, lockable &) throw(); //! kernel for thread
            void loop(parallel &context) throw(); //! the SIMD loop, called
            threads      workers;
        public:
            mutex       &access; //!< shared access
        private:
            bool         done;
            size_t       ready;         //!< count who is ready
            condition    synchronized;  //!< waiting to start cycle
            condition    cycle;

            kernel       kproc;
            void        *kdata;
        public:
            bool         &verbose;
        };

    }
}

#endif
