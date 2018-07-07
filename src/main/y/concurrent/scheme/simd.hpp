//! \file
#ifndef Y_CONCURRENT_SIMD_INCLUDED
#define Y_CONCURRENT_SIMD_INCLUDED 1

#include "y/concurrent/threads.hpp"
#include "y/concurrent/scheme/for-each.hpp"

namespace upsylon
{
    namespace concurrent
    {

        //! Single Instruction Multiple Data
        class simd :  public threads
        {
        public:
            typedef void (*kernel)( void *, parallel &, lockable & ); //!< the kernel prototype

            explicit simd(const bool v=false);
            virtual ~simd() throw();

            //! execute multiple copy of the kernel
            virtual void start( kernel user_code, void *user_data );
            virtual void finish() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(simd);
            const size_t threshold; //!< threads.size() + 1
            size_t       counter;   //!< for cycle control
            condition    cycle;     //!< to wait...
            kernel       code;
            void        *data;
            virtual void run(parallel &) throw();
        };
    }
}

#endif
