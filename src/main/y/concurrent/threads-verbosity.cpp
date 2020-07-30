
#include "y/concurrent/threads.hpp"
#include "y/string/env.hpp"
#include "y/string/convert.hpp"

#include <iostream>

namespace upsylon
{
    namespace concurrent
    {
        
        bool threads:: get_verbosity()
        {
            return environment::flag(Y_THREADS_VERBOSITY);
        }

    }

}

