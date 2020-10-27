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
            explicit           simd();              //!< construct threads
            virtual           ~simd() throw();      //!< destructor
            virtual void       run(kernel, void *); //!<  execute multiple copy of the kernel
            virtual executor & engine() throw();    //!< the underlying threads

        private:
            Y_DISABLE_COPY_AND_ASSIGN(simd);
            static  void call( void *, parallel &, lockable &) throw(); //! kernel for threads
            void loop(parallel &context) throw(); //! the SIMD loop, called
            threads      workers;

        public:
            mutex       &access; //!< shared access

        private:
            bool         done;
            size_t       ready;  //!< count who is ready
            condition    cycle;  //!< waiting to start cycle
            condition    synch;  //!< wait for end of cycle sync
            kernel       kproc;  //!< the Single Instruction
            void        *kdata;  //!< global data to share for kproc
            size_t       ended;  //!< count for waiting

        public:
            bool         &verbose; //!< for threads verbosity indicator
        };

    }
}

#endif
