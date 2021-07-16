//! \file

#ifndef Y_CONCURRENT_LOOP_ENV_INCLUDED
#define Y_CONCURRENT_LOOP_ENV_INCLUDED 1

#include "y/concurrent/loop/types.hpp"


namespace upsylon
{

    namespace concurrent
    {
        //______________________________________________________________________
        //
        //
        //! create a default looper from environment
        //
        //______________________________________________________________________
        struct looper_from_env
        {
            static const char var[]; //!< "Y_LOOPER"

            //! create a looper
            /**
             - if Y_LOOPER is "mono", "seq", "solo", "0" => solo()
             - if Y_LOOPER is "1..n" => simt(0,n,1)
             - default: simt() using Y_NUM_THREADS
             */
            static looper    *get();
        };
    }

}

#endif

