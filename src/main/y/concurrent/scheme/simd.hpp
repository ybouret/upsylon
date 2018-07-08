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
            virtual void start( kernel code, void *data );

            //! for_each interface: use internal barrier and wait
            virtual void finish() throw();

            //! the underlying threads
            virtual executor & engine() throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(simd);
            static  void call( void *, parallel &, lockable &) throw();
            void loop(parallel &context) throw();
            threads      workers;
            mutex       &access;
            bool         done;
            condition    synchronized;  //!< waiting to start cycle
            size_t       ready;         //!< count who is ready
            condition    cycle;
            const size_t threshold; //!< threads+1
            size_t       countdown; 
            kernel kproc;
            void  *kdata;
        };

    }
}

#endif
