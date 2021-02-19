
//! \file

#ifndef Y_CONCURRENT_LOOP_RUNNABLE_INCLUDED
#define Y_CONCURRENT_LOOP_RUNNABLE_INCLUDED 1

#include "y/concurrent/context.hpp"

//! VERBOSITY
#define Y_VERBOSE_SIMT "Y_VERBOSE_SIMT"

//! macro helper
#define Y_SIMT_LN(MSG) do { if(verbose) { std::cerr << MSG << std::endl; } } while(false)

namespace upsylon
{

    namespace concurrent
    {
        //!interface to runnable object
        class runnable
        {
        public:
            virtual ~runnable() throw();
            explicit runnable() throw();

            virtual void run(const context &, lockable &) = 0;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(runnable);
        };

    }

}

#endif
