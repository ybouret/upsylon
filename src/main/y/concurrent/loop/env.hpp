//! \file

#ifndef Y_CONCURRENT_LOOP_ENV_INCLUDED
#define Y_CONCURRENT_LOOP_ENV_INCLUDED 1

#include "y/concurrent/loop/types.hpp"


namespace upsylon
{

    namespace concurrent
    {

        //! create a default looper from environment
        struct looper_from_env
        {
            static const char var[]; //!< "Y_LOOPER"

            //! create a looper
            /**
             - if Y_LOOPER is "mono", "seq", "solo", "0" => mono
             - if Y_LOOPER is "1..n" => 
             */
            static looper    *get();
        };
    }

}

#endif

