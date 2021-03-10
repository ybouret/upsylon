
//! \file

#ifndef Y_CONCURRENT_LOOP_RUNNABLE_INCLUDED
#define Y_CONCURRENT_LOOP_RUNNABLE_INCLUDED 1

#include "y/concurrent/context.hpp"


//! macro helper
#define Y_SIMT_LN(MSG) do { if(verbose) { std::cerr << MSG << std::endl; } } while(false)

namespace upsylon
{

    namespace concurrent
    {
        //______________________________________________________________________
        //
        //
        //!interface to runnable object
        //
        //______________________________________________________________________
        class runnable
        {
        public:
            virtual ~runnable() throw(); //!< cleanup

            //! run code with one context, and a synchronizing object if necessary
            virtual void run(const context &, lockable &) throw() = 0;

        protected:
            explicit runnable() throw(); //!< setup
        private:
            Y_DISABLE_COPY_AND_ASSIGN(runnable);
        };

    }

}

#endif
