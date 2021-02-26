#include "y/concurrent/nucleus/thread.hpp"
#include "y/string/env.hpp"
#include <iostream>

namespace upsylon
{

    namespace concurrent
    {

        namespace  nucleus
        {
            bool thread:: verbosity(const char *id)
            {
                assert(id);
                const bool ans =  environment::flag(id);
                return ans;
            }

        }

    }

}

