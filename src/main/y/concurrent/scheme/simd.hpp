//! \file
#ifndef Y_CONCURRENT_SIMD_INCLUDED
#define Y_CONCURRENT_SIMD_INCLUDED 1

#include "y/concurrent/threads.hpp"

namespace upsylon
{
    namespace concurrent
    {

        //! Single Instruction Multiple Data
        class simd : public threads
        {
        public:
            typedef void (*kernel)( void *, parallel &, lockable & ); //!< the kernel prototype

            explicit simd(const bool v=false);
            virtual ~simd() throw();

            //! execute multiple copy of the kernel
            void ld( kernel user_code, void *user_data );


        private:
            Y_DISABLE_COPY_AND_ASSIGN(simd);
            kernel    code;
            void     *data;
            virtual void run(parallel &) throw();
        };
    }
}

#endif
